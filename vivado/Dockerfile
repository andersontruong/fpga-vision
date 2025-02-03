FROM archlinux:base-devel

LABEL maintainer "Anderson Truong <andersonleetruong@gmail.com>"

# ARG XILINX_VIVADO_VERSION=2024.1
# ARG XILINX_VIVADO_INSTALLER=FPGAs_AdaptiveSoCs_Unified_2024.1_0522_2023_Lin64.bin
# ARG XLNX_VIVADO_AUTH_FILE=wi_authentication_key
# ARG XILINX_VIVADO_BATCH_CONFIG_FILE=install_config.txt
#
# ENV XILINX_VIVADO_VERSION=${XILINX_VIVADO_VERSION}
# ENV XILINX_INSTALL_LOCATION=/tools/Xilinx
#
# COPY ${XILINX_VIVADO_INSTALLER} ${XILINX_INSTALL_LOCATION}/tmp/${XILINX_VIVADO_INSTALLER}
# COPY ${XILINX_VIVADO_BATCH_CONFIG_FILE} ${XILINX_INSTALL_LOCATION}/tmp/${XILINX_VIVADO_BATCH_CONFIG_FILE}
# COPY ${XLNX_VIVADO_AUTH_FILE} /root/.Xilinx/wi_authentication_key

RUN pacman -Syy --disable-sandbox

# RUN cd ${XILINX_INSTALL_LOCATION}/tmp/ && \
#     chmod +x ${XILINX_VIVADO_INSTALLER} && \
#     ./${XILINX_VIVADO_INSTALLER} -- \
#         --agree XilinxEULA,3rdPartyEULA \
#         --batch Install \
#         --config ${XILINX_INSTALL_LOCATION}/tmp/${XILINX_VIVADO_BATCH_CONFIG_FILE} && \
#     rm -rf ${XILINX_INSTALL_LOCATION}/tmp

CMD /bin/bash
