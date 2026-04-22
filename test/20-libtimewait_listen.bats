#!/usr/bin/env bats

@test "libtimewait_listen: socket options set" {
  run env LD_PRELOAD=./libtimewait_listen.so timeout 3 strace -e trace=network nc -l 0
  cat << EOF
--- output
$output
--- output
EOF

  [ "$status" -ne 0 ]

  SO_REUSEADDR='setsockopt\([0-9]*, SOL_SOCKET, SO_REUSEADDR, \[1\], 4\) = 0'
  SO_REUSEPORT='setsockopt\([0-9]*, SOL_SOCKET, SO_REUSEPORT, \[1\], 4\) = 0'

  [[ $output =~ $SO_REUSEADDR ]]

  case "$(uname -s)" in
  Linux)
    [[ $output =~ $SO_REUSEPORT ]]
    ;;
  *)
    ;;
  esac
}
