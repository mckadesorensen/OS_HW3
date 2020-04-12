# McKade Sorensen
# For CS321 homework 3

import re
import os
import time
from argparse import ArgumentParser
from typing import Dict
from subprocess import PIPE, call

MEM_REGEX = r"Size:([0-9]+)Stride:([0-9]+)read\+write:([0-9]+)\.([0-9]+)ns"


def parse_memory_out() -> Dict:
    parsed_data = []
    unparsed_data = grab_stats()

    for line in unparsed_data.splitlines():
        line_without_spaces = re.sub(" ", "", line)
        match = re.match(MEM_REGEX, line_without_spaces)
        if match:
            size, stride, speed_one, speed_two = match.groups()
            stats = {
                "size": size,
                "stride": stride,
                "read+write": float(f"{speed_one}.{speed_two}")
            }
            parsed_data.append(stats)

    return parsed_data


def grab_stats() -> str:
    with open("stats.txt") as file:
        data = file.read()
    return data


def let_program_finish() -> None:
    while True:
        if os.path.isfile("run"):
            break
        time.sleep(5)


def run_c_memory_program(c_program: str) -> None:
    call(f"gcc {c_program} -o run".split())
    let_program_finish()
    cmd = "./run > stats.txt".split()

    print("Executing c program...")
    with open('stats.txt', 'w') as out:
        call(cmd, stdout=out)
    os.remove("run")


def do_this_for_me(c_program: str) -> None:
    run_c_memory_program(c_program)
    data = parse_memory_out()
    print(data)


if __name__ == "__main__":
    p = ArgumentParser()
    p.add_argument("program", default="memory.c", help="c program to run")

    arguments = p.parse_args()
    do_this_for_me(arguments.program)
