#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC="${SCRIPT_DIR}/task1_2.cpp"
BIN="${SCRIPT_DIR}/task1_2_test_bin"

printf 'Compiling %s...\n' "${SRC}"
g++ -std=c++17 -O2 -pipe -Wall -Wextra -Werror -o "${BIN}" "${SRC}"

status=0
for input_file in "${SCRIPT_DIR}"/tests/input*.txt; do
    if [[ ! -f "${input_file}" ]]; then
        continue
    fi
    test_name=$(basename "${input_file}" .txt)
    expected_file="${input_file/input/output}"
    if [[ ! -f "${expected_file}" ]]; then
        echo "Missing expected output for ${test_name}" >&2
        status=1
        continue
    fi

    tmp_out=$(mktemp)
    "${BIN}" < "${input_file}" > "${tmp_out}"
    if ! diff -u "${expected_file}" "${tmp_out}"; then
        echo "Test ${test_name} failed." >&2
        status=1
    else
        echo "Test ${test_name} passed."
    fi
    rm -f "${tmp_out}"
done

if [[ ${status} -eq 0 ]]; then
    echo "All tests passed."
else
    echo "Some tests failed." >&2
fi

exit ${status}
