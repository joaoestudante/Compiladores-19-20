import os, sys
import subprocess

err = []
diff = []

def get_diff(file1, file2):
    with open(file1) as f1:
        f1_text = f1.read().splitlines()
    with open(file2) as f2:
        f2_text = f2.read().splitlines()

    if (f1_text != f2_text):
        diff.append("{}\n  expected:\n    {!r}\n  got:\n    {!r}\n".format(file1, f2_text, f1_text))
        return True

    return False


def _run_command(command, state):
    result = subprocess.run(command, shell=True, capture_output=True)
    if result.stderr:
        raise subprocess.CalledProcessError(
                returncode = result.returncode,
                cmd = result.args,
                stderr = result.stderr
                )
    if result.stdout and state == "compiling":
        raise subprocess.CalledProcessError(
                returncode = result.returncode,
                cmd = result.args,
                stderr = result.stdout
                )

    return result


def process_file(filename):
    try:
        _run_command("./og -g --target asm auto-tests/{}".format(filename), "compiling")
        _run_command("yasm -felf32 auto-tests/{}.asm".format(filename[:-3]), "compiling")
        _run_command("ld -melf_i386 -o main {}.o -lrts".format(filename[:-3]), "compiling")
        _run_command("./main > auto-tests/{}.out".format(filename[:-3]), "")
        if(get_diff("auto-tests/{0}.out".format(filename[:-3]), "auto-tests/expected/{0}.out".format(filename[:-3]))):
            return 0

        return 1

    except subprocess.CalledProcessError as error:
        error_str = error.stderr.decode("utf-8")
        err.append("{} - {}".format(filename, ', '.join(error_str.split("\n"))[:-2]))
        return 0


def main():
    os.system("make")
    total_tests = 0
    success = 0;

    single_test = False
    if len(sys.argv) > 1 and sys.argv[1].endswith("og"):
        total_tests += 1
        filename = sys.argv[1].split('/')[-1]
        print("Processing: {}".format(filename))
        success += process_file(filename)
        single_test = True

    if not single_test:
        for filename in sorted(os.listdir("./auto-tests")):
            if filename.endswith("og"):
                total_tests += 1
                print("Processing: {}".format(filename))
                success += process_file(filename)

    print("")
    print("########## RESULTS - PROBLEMS ##########")
    for i in err:
        print("  * " + i)

    print("")
    print("######### RESULTS - DIFFERENT OUTPUT #########")
    for i in diff:
        print("  * " + i)

    print("")

    grade = success*100/total_tests
    if grade < 50:
        color = '\033[91m'
    elif grade >= 50 and grade < 90:
        color = '\033[93m'
    elif grade >= 90 and grade <= 100:
        color = '\033[92m'

    print("{}Grade: {:.2f}%".format(color, grade))
    #os.system("rm *ok.o")

main()
