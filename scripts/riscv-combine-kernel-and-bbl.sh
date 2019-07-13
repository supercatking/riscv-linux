#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# Copyright (C) 2019 SiFive, Inc.
#
# This is intended to be called from the Linux Makefile via a command line
# similar to:
#
# RISCV_PK_PATH=../riscv-pk/ RISCV_PK_CONFIG_METHOD=hifive_unleashed RISCV_PK_DTB=$(pwd)/arch/riscv/boot/dts/sifive/hifive-unleashed-a00.dtb  make -j$(nproc) vmlinux.bbl
#
# This assumes that a -linux-gnu cross compiler has been built, and
# the ARCH and CROSS_COMPILE environment variables have been set
# appropriately, and that an appropriate version of riscv-pk has been
# checked out into the directory above the kernel.
#

set -eu

OBJPATH="$(realpath $1)"
if [ ! -d "${OBJPATH}" ]; then
    echo Error: output object file path ${OBJPATH} does not exist
    exit 251
fi
RISCV_PK_PATH="$(realpath $2)"
if [ ! -d "${RISCV_PK_PATH}" ]; then
    echo Error: riscv-pk does not exist at ${RISCV_PK_PATH}
    exit 252
fi
RISCV_PK_CONFIG_METHOD="$3"
RISCV_PK_DTB="$4"
if [ ! -f "$(realpath ${RISCV_PK_DTB})" ]; then
    echo Error: DTB does not exist at ${RISCV_PK_DTB}
    exit 252
fi

#

VMLINUX_STRIPPED=$(tempfile)
"${CROSS_COMPILE}strip" -o "${VMLINUX_STRIPPED}" "${OBJPATH}/vmlinux"

RISCV_PK_OBJPATH="${OBJPATH}/riscv-pk"
rm -rf "${RISCV_PK_OBJPATH}"
mkdir -p "${RISCV_PK_OBJPATH}"
ORIGDIR="$(pwd)"
cd "${RISCV_PK_OBJPATH}"

RISCV_PK_HOST=$(${CROSS_COMPILE}gcc --version | head -1 | cut -f1 -d' ' | cut -f1-4 -d- )
CC="${CROSS_COMPILE}gcc" "${RISCV_PK_PATH}/configure" -v \
  --host=$RISCV_PK_HOST \
  --enable-print-device-tree \
  --with-payload="${VMLINUX_STRIPPED}" \
  --with-config-method="${RISCV_PK_CONFIG_METHOD}" \
  --enable-dtb --with-dtb-path="${RISCV_PK_DTB}"

CFLAGS="-mabi=lp64d -march=rv64imafdc" make -j$(nproc) -C "${RISCV_PK_OBJPATH}"

"${CROSS_COMPILE}objcopy" -S -O binary --change-addresses -0x80000000 bbl "${OBJPATH}/vmlinux.bbl"

rm -f "${VMLINUX_STRIPPED}"

echo Now write to the SD card with something like:
/bin/echo -e \\tsudo dd if="${OBJPATH}/vmlinux.bbl" of=OUTPUT-PARTITION bs=1M conv=nocreat
