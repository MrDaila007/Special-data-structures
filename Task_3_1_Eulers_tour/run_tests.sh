#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_DIR="${SCRIPT_DIR}/tests"
SOURCE="${SCRIPT_DIR}/task3_1.cpp"
BIN="${SCRIPT_DIR}/task3_1_runner"
TMP_OUT="${SCRIPT_DIR}/.task3_1_out"

if [[ ! -d "${TEST_DIR}" ]]; then
    echo "Тесты не найдены в ${TEST_DIR}" >&2
    exit 1
fi

g++ -std=c++17 -O2 "${SOURCE}" -o "${BIN}"

total=0
for input in "${TEST_DIR}"/*.in; do
    [[ -e "${input}" ]] || continue
    name="$(basename "${input}" .in)"
    expected="${TEST_DIR}/${name}.out"

    if [[ ! -f "${expected}" ]]; then
        echo "Нет ожидаемого выхода для теста ${name}" >&2
        rm -f "${BIN}" "${TMP_OUT}"
        exit 1
    fi

    "${BIN}" <"${input}" >"${TMP_OUT}"

    if ! diff -u "${expected}" "${TMP_OUT}" >"${SCRIPT_DIR}/.task3_1_diff"; then
        echo "Тест ${name} НЕ ПРОЙДЕН" >&2
        cat "${SCRIPT_DIR}/.task3_1_diff" >&2
        rm -f "${BIN}" "${TMP_OUT}" "${SCRIPT_DIR}/.task3_1_diff"
        exit 1
    fi

    rm -f "${SCRIPT_DIR}/.task3_1_diff"
    ((total += 1))
done

rm -f "${BIN}" "${TMP_OUT}"
echo "Все ${total} тестов успешно пройдены."
