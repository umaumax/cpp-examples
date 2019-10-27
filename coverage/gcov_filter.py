#!/usr/bin/env python3

import argparse
import re
import sys


class Gcov:
    def __init__(self, function_format='linux', verbose=False):
        self.verbose = verbose
        self.function_format = function_format

    def filter(self, input_filepath, output_filepath):
        re_function = re.compile(r"Function '(?P<func_name>[a-zA-Z0-9_]+)'")
        re_function_replacer = re.compile(r"^_+")
        re_lines_executed = re.compile(r"Lines executed:(?P<ratio>[0-9]+\.[0-9]+)% of (?P<line_of_func>[0-9]+)")
        lines = []
        with open(input_filepath, mode='r') as input_file:
            lines = input_file.read().splitlines()
        with open(output_filepath, mode='w') as output_file:
            i = 0
            for tmp_i in range(max(len(lines) - 1, 0)):
                if tmp_i < i:
                    continue
                i = tmp_i
                line = lines[i]
                next_line = lines[i + 1]
                match_function = re_function.fullmatch(line)
                if self.verbose:
                    print("# [DEBUG] {0}".format(line), file=sys.stderr)
                if match_function:
                    func_name = match_function.group('func_name')
                    # NOTE: _xxx -> __xxx when using mac
                    if self.function_format in ['mac', 'darwin'] and re_function_replacer.match(func_name):
                        func_name = '_' + func_name
                    match_lines_executed = re_lines_executed.fullmatch(next_line)
                    if match_lines_executed:
                        ratio = float(match_lines_executed.group('ratio'))
                        line_of_func = float(match_lines_executed.group('line_of_func'))
                        if ratio != 100.0:
                            # non-covered funcs
                            output_file.write("{0}\n".format(func_name))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', '--output-filepath', default='/dev/stdout', help='output_filepath')
    parser.add_argument('-v', '--verbose', action='store_true')
    parser.add_argument('--function-format', default='linux', help='function output format: linux | darwin(mac)')
    parser.add_argument('input_filepath', help='input file path (output of gcov)')
    parser.add_argument('args', nargs='*', help='1st args can overwrite output_filepath')  # any length of args is ok

    args, extra_args = parser.parse_known_args()
    if len(args.args) > 0:
        args.output_filepath = args.args[0]

    gcov = Gcov(function_format=args.function_format, verbose=args.verbose)
    gcov.filter(args.input_filepath, args.output_filepath)


if __name__ == '__main__':
    main()
