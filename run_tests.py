#!/usr/local/bin/python3
import os
import subprocess
import time
import pexpect


def spawn_xv6():
    return pexpect.spawn(" ".join([
            "qemu-system-i386",
            "-drive", "file=xv6.img,media=disk,index=0,format=raw",
            "-drive", "file=fs.img,index=1,media=disk,format=raw",
            "-smp", "2",
            "-m", "512",
            "-display", "none",
            "-nographic",
        ]), maxread=30000, encoding="utf-8")


def run_test(src_dir: str) -> bool:
    stdin_pipes = []
    with open("{}/commands.in".format(src_dir), "r") as cfile:
        stdin_pipes = cfile.readlines()

    xv6_session = spawn_xv6()
    result1 = xv6_session.expect([pexpect.TIMEOUT, r".*\r\ninit: starting sh\r\n\$ "], timeout=5)
    if result1 == 0:
        print("BOOT TIMEOUT")
        xv6_session.terminate(force=True)
        return False
    
    for l in stdin_pipes:
        xv6_session.send(l)
        xv6_session.readline() # ignore the terminal output that it sends back on keypress.
    
    expecteds = []
    with open("{}/expected.out".format(src_dir), "rb") as efile:
        expecteds = efile.readlines()

    for i in range(len(expecteds)):
        actual = xv6_session.readline()
        if expecteds[i].decode("utf-8") == actual:
            continue
        else:
            print("\tUNEXPECTED RESULT LINE", i)
            print("\tGOT:")
            print("\t\t{}".format(repr(actual)))
            print("\tEXPECTED:")
            print("\t\t{}".format(repr(expecteds[i].decode("utf-8"))))
            xv6_session.terminate(force=True)
            return False
    return True


def main():
    no_failures = True
    try:
        test_dirs = os.listdir("./tests")
        if len(test_dirs) > 0:
            print("Starting Tests...\n")
        
        for dir in sorted(test_dirs):
            if os.path.isdir("./tests/{}".format(dir)) and dir.startswith("test_"):
                result = run_test("tests/{}".format(dir))
                if result:
                    print("SUCCESS - {}".format(dir))
                else:
                    no_failures = False
                    print("FAILURE - {}".format(dir))
                print("="*10)
    except:
        no_failures = False
    print("Testing Finished", end="")
    if no_failures:
        print(" with no failures.")
    else:
        print(" with failures.")



if __name__ == "__main__":
    main()