#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_FILE="${SCRIPT_DIR}/task.cpp"
BINARY_FILE="${SCRIPT_DIR}/task"
TEST_DIR="${SCRIPT_DIR}/tests"

if [[ ! -d "${TEST_DIR}" ]]; then
    echo "No tests directory found at ${TEST_DIR}" >&2
    exit 1
fi

shopt -s nullglob
test_inputs=("${TEST_DIR}"/*.in)
shopt -u nullglob

if [[ "${#test_inputs[@]}" -eq 0 ]]; then
    echo "No test input files found in ${TEST_DIR}" >&2
    exit 1
fi

echo "Compiling ${SOURCE_FILE}..."
g++ -std=c++17 -O2 -pipe "${SOURCE_FILE}" -o "${BINARY_FILE}"

overall_status=0

for input_file in "${test_inputs[@]}"; do
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

    if "${BINARY_FILE}" < "${input_file}" > "${actual_file}"; then
        if diff -u "${expected_file}" "${actual_file}" > /dev/null; then
            echo "  OK ${test_name}"
            rm -f "${actual_file}"
        else
            echo "  FAIL ${test_name}"
            overall_status=1
        fi
    else
        echo "  FAIL ${test_name} (execution error)"
        overall_status=1
    fi
done

rm -f "${BINARY_FILE}"

exit "${overall_status}"
