#!/bin/bash

set -o errexit
set -o pipefail
set -o nounset


main() {
    local testdir="$(dirname $(readlink --canonicalize "$0"))"
    pushd . >/dev/null
    cd "$testdir"
    diff test_expected.txt <(./test)
    popd >/dev/null
    echo "Tests passed!"
}


main "$@"

