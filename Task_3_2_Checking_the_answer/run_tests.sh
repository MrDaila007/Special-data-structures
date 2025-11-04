#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_DIR="${SCRIPT_DIR}/tests"
SOURCE="${SCRIPT_DIR}/task3_2.cpp"
BIN="${SCRIPT_DIR}/task3_2_runner"
TMP_OUT="${SCRIPT_DIR}/.task3_2_actual"

if [[ ! -d "${TEST_DIR}" ]]; then
    echo "Тесты не найдены в ${TEST_DIR}" >&2
    exit 1
fi

g++ -std=c++17 -O2 "${SOURCE}" -o "${BIN}"

total=0
for input in "${TEST_DIR}"/*.in; do
    [[ -e "${input}" ]] || continue
    ((total += 1))
    name="$(basename "${input}" .in)"
    expected="${TEST_DIR}/${name}.out"

    if [[ ! -f "${expected}" ]] || [[ ! -s "${expected}" ]]; then
        echo "Нет ожидаемого вывода для теста ${name}" >&2
        rm -f "${BIN}" "${TMP_OUT}"
        exit 1
    fi

    if ! "${BIN}" < "${input}" > "${TMP_OUT}"; then
        echo "Тест ${name} завершился с ошибкой выполнения" >&2
        rm -f "${BIN}" "${TMP_OUT}"
        exit 1
    fi

    if ! diff -u "${expected}" "${TMP_OUT}" > "${SCRIPT_DIR}/.diff_tmp"; then
        echo "Тест ${name} НЕ ПРОЙДЕН" >&2
        cat "${SCRIPT_DIR}/.diff_tmp" >&2
        rm -f "${BIN}" "${TMP_OUT}" "${SCRIPT_DIR}/.diff_tmp"
        exit 1
    fi

    rm -f "${SCRIPT_DIR}/.diff_tmp"
done

rm -f "${BIN}" "${TMP_OUT}"

echo "Все ${total} тестов успешно пройдены."
