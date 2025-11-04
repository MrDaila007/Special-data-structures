#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_FILE="${SCRIPT_DIR}/task1.1.cpp"
BINARY_FILE="${SCRIPT_DIR}/task1.1"
TEST_DIR="${SCRIPT_DIR}/tests"

if [[ ! -d "${TEST_DIR}" ]]; then
    echo "No tests directory found at ${TEST_DIR}" >&2
    exit 1
fi

echo "Compiling ${SOURCE_FILE}..."
g++ -std=c++17 -O2 -pipe "${SOURCE_FILE}" -o "${BINARY_FILE}"

overall_status=0

for input_file in "${TEST_DIR}"/*.in; do
    [[ -e "${input_file}" ]] || continue
    base_name="${input_file%.in}"
    expected_file="${base_name}.out"
    actual_file="${base_name}.actual"

    if [[ ! -f "${expected_file}" ]]; then
        echo "Missing expected output for ${input_file}" >&2
        overall_status=1
        continue
    fi

    test_name="$(basename "${input_file}")"
    echo "Running ${test_name}..."
    if "${BINARY_FILE}" < "${input_file}" > "${actual_file}" && diff -u "${expected_file}" "${actual_file}" > /dev/null; then
        echo "  OK ${test_name}"
        rm -f "${actual_file}"
    else
        echo "  FAIL ${test_name}"
        overall_status=1
    fi
done

rm -f "${BINARY_FILE}"

exit "${overall_status}"
