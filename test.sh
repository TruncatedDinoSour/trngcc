#!/usr/bin/env sh

set -e

OUTDIR="${OUTDIR:-tests-out}"

log() { "${2:-echo}" " :: $1"; }

test_gcc_like() {
    case "$2" in
    c)
        log "\033[1mTesting C with $3\033[0m\n" printf

        for cstd in c90 c89 iso9899:1990 iso9899:199409 \
            c99 c9x iso9899:1999 iso9899:199x c11 c1x \
            iso9899:2011 c17 c18 iso9899:2017 iso9899:2018 \
            c2x iso9899:2018 c2x gnu90 gnu89 gnu99 gnu9x gnu11 \
            gnu1x gnu17 gnu18 gnu2x; do
            {
                log "Testing C standard: \033[1m$cstd\033[0m\n" printf

                if ! "$3" -Iinclude -Werror -Wall -Wextra -Wshadow -Wpedantic \
                    -pedantic -std="$cstd" -o "$OUTDIR/$3-$cstd" "$1"; then
                    log "\033[1;31mC standard $cstd failed\033[0m\n" printf >&2
                    exit 2
                fi
            } &
        done

        wait
        ;;
    cpp)
        log "\033[1mTesting C++ with $4\033[0m\n" printf

        for cppstd in c++98 c++03 gnu++98 gnu++03 c++11 c++0x \
            gnu++11 gnu++0x c++14 c++1y gnu++14 gnu++1y c++17 \
            c++1z gnu++17 gnu++1z c++20 c++2a gnu++20 gnu++2a \
            c++2b gnu++2b; do
            {
                log "Testing C++ standard: \033[1m$cppstd\033[0m\n" printf

                if ! "$4" -Iinclude -Werror -Wall -Wextra -Wshadow -Wpedantic \
                    -pedantic -std="$cppstd" -o "$OUTDIR/$4-$cppstd" "$1"; then
                    log "\033[1;31mC++ standard $cppstd failed\033[0m\n" printf >&2
                    exit 2
                fi
            } &
        done

        wait
        ;;
    *)
        log "Unhandled test case: $1" >&2
        exit 1
        ;;
    esac
}

test_gcc() { test_gcc_like "$1" "$2" 'gcc' 'g++'; }

test_clang() { test_gcc_like "$1" "$2" 'clang' 'clang++'; }

test_tcc() {
    if [ "$2" = 'c' ]; then
        log '\033[1mTesting C with tcc\033[0m\n' printf

        for std in c99 c11; do
            {
                log "Testing C standard: \033[1m$cstd\033[0m\n" printf

                if ! tcc -Iinclude -std="$std" -Wall -Werror -o "$OUTDIR/tcc-$std" "$1"; then
                    log "\033[1;31mC standard $std failed\033[0m\n" printf >&2
                    exit 2
                fi
            } &
        done

        wait
    fi
}

run_binaries() {
    for bin in "$OUTDIR"/*; do
        {
            log "Running \033[1m$bin\033[0m\n" printf

            if ! ./"$bin" >/dev/null 2>&1; then
                log "\033[1;31mBinary '$bin' failed\033[0m\n" printf >&2
                exit 3
            fi
        } &
    done

    wait
}

main() {
    rm -rf -- "$OUTDIR"
    mkdir -p -- "$OUTDIR"

    for compiler in gcc clang tcc; do
        log "Looking for '$compiler'... " printf
        if ! command -v "$compiler"; then
            echo 'not found'
            continue
        fi

        for testf in tests/*; do
            "test_$compiler" "$testf" "$(printf '%s' "$testf" |
                awk -F . '{if (NF>1) {print $NF}}')"
        done
    done

    log 'Running binaries'
    run_binaries
}

main "$@"
