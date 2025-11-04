#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_DIR="${SCRIPT_DIR}/tests"
SOURCE="${SCRIPT_DIR}/task48.cpp"
BIN="${SCRIPT_DIR}/task48_runner"

if [[ ! -d "${TEST_DIR}" ]]; then
    echo "Тесты не найдены в ${TEST_DIR}" >&2
    exit 1
fi

g++ -std=c++17 -O2 -pipe "${SOURCE}" -o "${BIN}"

rm -f "${SCRIPT_DIR}/archive.in" "${SCRIPT_DIR}/archive.out"

total=0
for input in "${TEST_DIR}"/*.in; do
    [[ -e "${input}" ]] || continue
    ((total += 1))
    name="$(basename "${input}" .in)"
    expected="${TEST_DIR}/${name}.out"

    if [[ ! -f "${expected}" ]]; then
        echo "Нет ожидаемого ответа для теста ${name}" >&2
        rm -f "${BIN}"
        exit 1
    fi

    cp "${input}" "${SCRIPT_DIR}/archive.in"
    (
        cd "${SCRIPT_DIR}"
        "./$(basename "${BIN}")" >/dev/null
    )

    if ! diff -u "${expected}" "${SCRIPT_DIR}/archive.out" >"${SCRIPT_DIR}/.diff_tmp"; then
        echo "Тест ${name} НЕ ПРОЙДЕН" >&2
        cat "${SCRIPT_DIR}/.diff_tmp" >&2
        rm -f "${SCRIPT_DIR}/archive.in" "${SCRIPT_DIR}/archive.out" "${SCRIPT_DIR}/.diff_tmp" "${BIN}"
        exit 1
    fi

    rm -f "${SCRIPT_DIR}/.diff_tmp" "${SCRIPT_DIR}/archive.out"
done

rm -f "${SCRIPT_DIR}/archive.in" "${BIN}"

echo "Все ${total} тестов успешно пройдены."
