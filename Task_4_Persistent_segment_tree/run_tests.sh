#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_FILE="${SCRIPT_DIR}/task4.cpp"
BINARY_FILE="${SCRIPT_DIR}/task4"
TEST_DIR="${SCRIPT_DIR}/tests"
INPUT_FILE="${SCRIPT_DIR}/input.txt"
OUTPUT_FILE="${SCRIPT_DIR}/output.txt"

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
    rm -f "${actual_file}"

    if [[ ! -f "${expected_file}" ]]; then
        echo "Missing expected output for ${input_file}" >&2
        overall_status=1
        continue
    fi

    test_name="$(basename "${input_file}")"
    echo "Running ${test_name}..."

    cp "${input_file}" "${INPUT_FILE}"

    if "${BINARY_FILE}"; then
        if [[ ! -f "${OUTPUT_FILE}" ]]; then
            echo "  FAIL ${test_name} (missing ${OUTPUT_FILE})"
            overall_status=1
        elif diff -u "${expected_file}" "${OUTPUT_FILE}" > /dev/null; then
            echo "  OK ${test_name}"
        else
            echo "  FAIL ${test_name}"
            cp "${OUTPUT_FILE}" "${actual_file}"
            overall_status=1
        fi
    else
        echo "  FAIL ${test_name} (execution error)"
        overall_status=1
    fi

    rm -f "${INPUT_FILE}" "${OUTPUT_FILE}"
done

rm -f "${BINARY_FILE}"

exit "${overall_status}"
