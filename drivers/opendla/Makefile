# Copyright (c) 2017, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

ccflags-$(CONFIG_OPENDLA) += -I$(srctree)/drivers/opendla/include
ccflags-$(CONFIG_OPENDLA) += -I$(srctree)/drivers/opendla

obj-$(CONFIG_OPENDLA) += scheduler.o
obj-$(CONFIG_OPENDLA) += engine.o
obj-$(CONFIG_OPENDLA) += bdma.o
obj-$(CONFIG_OPENDLA) += conv.o
obj-$(CONFIG_OPENDLA) += sdp.o
obj-$(CONFIG_OPENDLA) += cdp.o
obj-$(CONFIG_OPENDLA) += pdp.o
obj-$(CONFIG_OPENDLA) += rubik.o
obj-$(CONFIG_OPENDLA) += cache.o
obj-$(CONFIG_OPENDLA) += common.o
obj-$(CONFIG_OPENDLA) += engine_data.o
obj-$(CONFIG_OPENDLA) += engine_isr.o
obj-$(CONFIG_OPENDLA) += engine_debug.o
obj-$(CONFIG_OPENDLA) += nvdla_core_callbacks.o
obj-$(CONFIG_OPENDLA) += nvdla_gem.o
