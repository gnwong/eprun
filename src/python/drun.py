#!/home/gnwong/miniconda3/bin/python

"""
 * Copyright (c) 2024 George Wong
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
"""

import os
import fcntl
import sys
import time

from multiprocessing import Pool


def get_next_command(fname):
    """ Return the next command from the file and remove it safely. """
    commands = [None]
    with open(fname, 'r+') as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        commands = f.readlines()
        if not commands:
            return None
        f.seek(0)
        f.truncate()
        f.writelines(commands[1:])
        fcntl.flock(f, fcntl.LOCK_UN)
    return commands[0].strip()


def run(args):
    """ Pull commands from file and launch them. """
    index, fname = args
    time.sleep(index)
    while True:
        command = get_next_command(fname)
        if not command:
            print(' - no more commands to run')
            break
        print(f' - running command: {command}')
        os.system(command)


if __name__ == "__main__":

    commands_fname = sys.argv[1]

    n_parallel = 1
    if len(sys.argv) > 2:
        n_parallel = int(sys.argv[2])
        print(f' - launching {n_parallel} simultaneous processes')

    all_commands = []
    for i in range(n_parallel):
        all_commands.append((i, commands_fname))

    print(f' - running commands from: {commands_fname}')
    with Pool(processes=n_parallel) as pool:
        pool.map(run, all_commands)
    



