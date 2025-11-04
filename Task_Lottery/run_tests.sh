#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE="${SCRIPT_DIR}/task_lott.cpp"
BINARY="${SCRIPT_DIR}/task_lott_bin"
TEST_DIR="${SCRIPT_DIR}/tests"

if [[ ! -d "${TEST_DIR}" ]]; then
  echo "Test directory not found: ${TEST_DIR}" >&2
  exit 1
fi

if [[ -f "${BINARY}" ]]; then
  rm -f "${BINARY}"
fi

g++ -std=c++17 -O2 -pipe -Wall -Wextra -Wshadow -Wconversion -o "${BINARY}" "${SOURCE}"

temp_dir="$(mktemp -d)"
trap 'rm -rf "${temp_dir}" "${BINARY}"' EXIT

pass_count=0
test_count=0

for input in "${TEST_DIR}"/*.in; do
  [[ -e "${input}" ]] || continue
  test_name="$(basename "${input}" .in)"
  expected="${TEST_DIR}/${test_name}.out"
  if [[ ! -f "${expected}" ]]; then
    echo "Missing expected output for ${test_name}" >&2
    exit 1
  fi

  output_file="${temp_dir}/${test_name}.out"
  "${BINARY}" < "${input}" > "${output_file}"

  if diff -u "${expected}" "${output_file}" > /dev/null; then
    echo "${test_name}: OK"
    ((++pass_count))
  else
    echo "${test_name}: FAIL" >&2
    diff -u "${expected}" "${output_file}" >&2 || true
    exit 1
  fi
  ((++test_count))

done

echo "Passed ${pass_count}/${test_count} tests."
