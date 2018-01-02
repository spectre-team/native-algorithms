import glob
import os
from typing import List
from tqdm import tqdm


def find_headers() -> List[str]:
    return glob.glob('Spectre.*/*.h', recursive=True)

SOLUTION_INCLUDE_PREFIX = '#include "'

def is_local_include(line: str) -> bool:
    if not line.startswith(SOLUTION_INCLUDE_PREFIX):
        return False
    return not line.startswith('Spectre.', len(SOLUTION_INCLUDE_PREFIX))


def add_prefix(local_include_line: str, file_name: str) -> str:
    project_prefix = os.path.split(file_name)[0] + "/"
    include_target = local_include_line[len(SOLUTION_INCLUDE_PREFIX):]
    return SOLUTION_INCLUDE_PREFIX + project_prefix + include_target


def prefix_includes(header_content: List[str], file_name: str) -> List[str]:
    return [
        add_prefix(line, file_name) if is_local_include(line) else line
        for line in header_content
    ]


def main():
    header_files = find_headers()
    for file_name in tqdm(header_files):
        with open(file_name) as header:
            content = header.readlines()
        prefixed = prefix_includes(content, file_name)
        with open(file_name, 'w') as header:
            header.writelines(prefixed)


if __name__ == "__main__":
    main()
