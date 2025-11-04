#!/usr/bin/env bash

# Запускает все скрипты run_tests.sh во вложенных каталогах.
# Скрипт собирает краткую сводку и возвращает ненулевой код, если хоть один запуск завершился неуспешно.

set -uo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${ROOT_DIR}"

if ! command -v sort >/dev/null 2>&1; then
    echo "Не найден утилита sort, выполнение остановлено." >&2
    exit 127
fi

declare -a SUCCESS_TASKS=()
declare -a FAILED_TASKS=()

while IFS= read -r -d '' script_path; do
    rel_path="${script_path#"${ROOT_DIR}/"}"
    task_dir="$(dirname "${rel_path}")"

    echo ">>> Запуск тестов: ${task_dir}"

    if (cd "${task_dir}" && bash "./run_tests.sh"); then
        SUCCESS_TASKS+=("${task_dir}")
        echo ">>> Результат: OK ${task_dir}"
    else
        FAILED_TASKS+=("${task_dir}")
        echo ">>> Результат: FAIL ${task_dir}"
    fi

    echo
done < <(find "${ROOT_DIR}" -mindepth 2 -maxdepth 2 -name run_tests.sh -print0 | sort -z)

echo "====== ИТОГ ======"
echo "Успешно: ${#SUCCESS_TASKS[@]}"
for task in "${SUCCESS_TASKS[@]}"; do
    echo "  - ${task}"
done

echo "Провалено: ${#FAILED_TASKS[@]}"
for task in "${FAILED_TASKS[@]}"; do
    echo "  - ${task}"
done

[[ ${#FAILED_TASKS[@]} -eq 0 ]]
