import argparse
from pathlib import Path
import os


# Category: utility
# Wrapper on top of a file-like object that intercepts write calls
#   in order to respect the verbose flag value
# 0  - no output (verbose off)
# 1  - print number of written bytes at end of file
# 2  - print filename and how many bytes were written
# 3+ - print filename and actual bytes written
class TestWriter:
    def __init__(self, file, args):
        self.file = file
        self.filename = Path(self.file.name).name
        self.args = args
        self.lines = 0
        self.total_bytes = 0
        pass

    def write(self, _bytes):
        self.file.write(_bytes)
        if self.args.verbose == 0:
            # no verbose output
            pass
        elif self.args.verbose == 1:
            # do nothing here, delegate to __exit__
            pass
        elif self.args.verbose == 2:
            # filename that is written to
            # number of bytes written
            print(f"  {self.filename}: {len(_bytes)}")
        else:
            # [2..]
            # filename that is written to
            # bytes written
            print(f"  {self.filename}: {_bytes}")
        self.lines += 1
        self.total_bytes += len(_bytes)

    def __enter__(self):
        return self

    # invoked when TestWriter instance goes out of scope in a with block
    def __exit__(self, exc_type, exc_value, traceback):
        if self.args.verbose == 1:
            print(f"   {self.filename}: Wrote {self.total_bytes} bytes")


# Category: generators
def genfile1(args, f):
    f.write(b'helo\n')
    pass


def genfile2(args, f):
    f.write(b'helo\n')


def genfile3(args, f):
    f.write(b'booger\n')
    f.write(b'face\n')


def genfile4(args, f):
    for i in range(500):
        f.write(b'.' * 100 + b'\n')
    pass


genmap = {
    'test1.txt': genfile1,
    'test2.txt': genfile2,
    'test3.txt': genfile3,
    'test4.txt': genfile4,
}


# Category: entry-point
def main():
    parser = argparse.ArgumentParser(prog='genfiles',
                                     description='generate test files',
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('-o',
                        '--outdir',
                        required=True,
                        help='where the test files should go')

    parser.add_argument('-v',
                        '--verbose',
                        action='count',
                        default=0,
                        help=("enable verbose output\n"
                              " -v   : print number of written bytes at end of file\n"
                              " -vv  : print filename and how many bytes were written\n"
                              " -vvv : print filename and actual bytes written\n"))

    args = parser.parse_args()

    for key, val in genmap.items():
        try:
            filepath = os.path.realpath(args.outdir + f'/{key}')
            dirname = os.path.dirname(filepath)
            os.makedirs(dirname, exist_ok=True)
            with open(filepath, 'wb') as f:
                with TestWriter(f, args) as w:
                    val(args, w)
        except Exception as e:
            print(f'error {e}')

    print(f'{os.path.basename(__file__)} : Generated {len(genmap)} files')
    pass


if __name__ == '__main__':
    main()
