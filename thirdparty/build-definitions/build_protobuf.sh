# Copyright (c) YugaByte, Inc.

PROTOBUF_VERSION=2.6.1
PROTOBUF_DIR=$TP_SOURCE_DIR/protobuf-$PROTOBUF_VERSION
TP_NAME_TO_SRC_DIR["protobuf"]=$PROTOBUF_DIR
TP_NAME_TO_ARCHIVE_NAME["protobuf"]="protobuf-${PROTOBUF_VERSION}.tar.gz"

build_protobuf() {
  create_build_dir_and_prepare "$PROTOBUF_DIR"
  (
    set_build_env_vars
    set_thirdparty_flags_for_autotools_projects
    set -x
    # We build protobuf in both instrumented and non-instrumented modes.
    # If we don't clean in between, we may end up mixing modes.
    autoreconf --force --install
    test -f Makefile && make distclean
    YB_REMOTE_BUILD=0 run_configure \
      --with-pic \
      --enable-shared \
      --enable-static \
      --prefix=$PREFIX
    run_make install
  )
}
