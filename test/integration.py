"""spg Integration Test

integration.py runs the spgencrypt and spgdecrypt binaries and ensures that it
executes correct and useful computations.

Requires
* path to spgencrypt in argv[1]
* path to spgdecrypt in argv[2]

"""

import codecs
from dataclasses import dataclass
from typing import Dict
import os
from pathlib import Path
import re
import subprocess
import sys


@dataclass
class Command:
  encrypt_key: str
  encrypt_message: str
  decrypt_key: str


@dataclass
class Test:
  """check the execute() method below for how this gets executed
  want maps file name regexps to lists of regexps we expect to match the content of the files.
  for file name regexps, stdout and stderr are special files."""
  name: str
  commands: list[Command]
  want: Dict[str, list[str]]


TEST_PASS_MESSAGE = 'PASS'
TEST_FAIL_MESSAGE = 'FAIL'

TESTS = [
    Test("TestEncryptFailOnEmptyKey", [Command("", "message", "key")],
         {"stdout": ["Missing encryption key"]}),
    Test("TestEncryptFailOnEmptyMessage", [Command("key", "", "key")],
         {"stdout": ["Missing message to encrypt"]}),
    Test("TestDecryptFailOnEmptyKey", [Command("key", "message", "")],
         {"stdout": ["Missing decryption key"]}),
    Test("TestDecryptFailOnWrongKey",
         [Command("key", "message", ""),
          Command("", "", "yek")], {"stdout": ["Failed to decrypt"]}),
    Test("TestEncryptFile", [Command("key", "message", "")], {
        "stdout": ["Encryption succeeded"],
        "spg$": ["hex"]
    }),
    Test("TestEncryptAndDecryptFile",
         [Command("key", "message", ""),
          Command("", "", "key")], {
              "stdout": ["message"],
              "spg$": ["hex"]
          }),
    Test("TestMultipleFilesDecryptTheOneAccessibleByKey", [
        Command("key1", "message1", ""),
        Command("key2", "message2", ""),
        Command("", "", "key1")
    ], {"stdout": ["message1"]}),
    Test("TestMultipleFilesDecryptAllMatches", [
        Command("key1", "message1", ""),
        Command("key1", "message2", ""),
        Command("", "", "key1")
    ], {"stdout": ["message(1|2)"]}),
]


def execute(t, encrypt_binary, decrypt_binary):
  passed = True
  got = {"stdout": ""}
  for c in t.commands:
    encrypt_call_args = [
        encrypt_binary, "-k", c.encrypt_key, "-m", c.encrypt_message
    ]
    decrypt_call_args = [decrypt_binary, "-k", c.decrypt_key]
    print(f"Calling encrypt: {encrypt_call_args}.")
    encrypt_call = subprocess.run(encrypt_call_args, capture_output=True)
    got["stdout"] += encrypt_call.stdout.decode()
    print(f"Calling decrypt: {decrypt_call_args}")
    decrypt_call = subprocess.run(decrypt_call_args, capture_output=True)
    got["stdout"] += decrypt_call.stdout.decode()
    if (encrypt_call.returncode == 0):
      generated_files = [
          f for f in os.listdir(Path(encrypt_binary).parent)
          if re.search(r'\.spg$', f, re.IGNORECASE)
      ]
      for f in generated_files:
        if f not in got:
          try:
            got[f] = open(f, "r").read()
          except UnicodeDecodeError as e:
            # TODO: this exception is thrown when trying to decode the hex dump
            # of the ciphertext as UTF8 hence we simply just store the word hex
            # for now.
            got[f] = "hex"
  got_fileset = set(got.keys())
  for want_f, want_list in t.want.items():
    relevant_filesubset = []
    for got_f in got_fileset:
      if re.search(want_f, got_f):
        relevant_filesubset.append(got_f)
    if (len(relevant_filesubset) == 0):
      print(
          f"Expected file with regex {want_f} in generated file list. Not found in {got_fileset} (ignore stdout)\n"
      )
      passed = False
      break
    for relevant_f in relevant_filesubset:
      got_content = got[relevant_f]
      for want in want_list:
        matches = re.findall(want, got_content)
        if (len(matches) == 0):
          print(f"Expected:\n{want}\n\nNot found in:\n{got_content}")
          passed = False
          break
  for f in got_fileset:
    Path(f).unlink(missing_ok=True)
  return passed


def test_spg():
  if len(sys.argv) != 3:
    print(f"ERROR: wrong number of args. "
          "* path to spgencrypt in argv[1]"
          "* path to spgdecrypt in argv[2]")
    return -1
  encrypt_binary = sys.argv[1]
  decrypt_binary = sys.argv[2]
  print(
      f"Test SPG Integration. Using encrypter: {encrypt_binary}. Using decrypter: {decrypt_binary}"
  )
  passed = 0
  for t in TESTS:
    test_status = TEST_FAIL_MESSAGE
    print(f"Running {t.name}")
    if (execute(t, encrypt_binary, decrypt_binary)):
      passed += 1
      test_status = TEST_PASS_MESSAGE
    print(f"\t{test_status} {t.name}\n")
  print(f"Final result: Passed {passed} out of {len(TESTS)}\n")


if __name__ == "__main__":
  test_spg()
