XILINX_VIVADO_VERSION=2024.1
XILINX_VIVADO_INSTALLER=FPGAs_AdaptiveSoCs_Unified_2024.1_0522_2023_Lin64.bin
XILINX_VIVADO_AUTH_FILE=wi_authentication_key
XILINX_VIVADO_BATCH_CONFIG_FILE=install_config.txt
XILINX_INSTALL_LOCATION=/tools/Xilinx

build:
	./${XILINX_VIVADO_INSTALLER} -- -b AuthTokenGen
	cp ${HOME}/.Xilinx/wi_authentication_key .

	docker build \
	-t vivado:${XILINX_VIVADO_VERSION} .
	# --build-arg XILINX_VIVADO_VERSION=${XILINX_VIVADO_VERSION} \
	# --build-arg XILINX_VIVADO_INSTALLER=${XILINX_VIVADO_INSTALLER} \
	# --build-arg XILINX_VIVADO_AUTH_FILE=${XILINX_VIVADO_AUTH_FILE} \
	# --build-arg XILINX_VIVADO_BATCH_CONFIG_FILE=${XILINX_VIVADO_BATCH_CONFIG_FILE} \

run:
	docker run -it -v ${PWD}/install/:/tools/Xilinx -v ${PWD}:/workspace vivado:${XILINX_VIVADO_VERSION}

install_vivado:
	mkdir -p ${XILINX_INSTALL_LOCATION}/tmp
	cp /workspace/${XILINX_VIVADO_INSTALLER} ${XILINX_INSTALL_LOCATION}/tmp
	cp /workspace/${XILINX_VIVADO_AUTH_FILE} ${XILINX_INSTALL_LOCATION}/tmp
	cp /workspace/${XILINX_VIVADO_BATCH_CONFIG_FILE} ${XILINX_INSTALL_LOCATION}/tmp
	mkdir -p /root/.Xilinx
	cp ${XILINX_VIVADO_AUTH_FILE} /root/.Xilinx/wi_authentication_key
	cd ${XILINX_INSTALL_LOCATION}/tmp/ && \
	./${XILINX_VIVADO_INSTALLER} -- \
	    --agree XilinxEULA,3rdPartyEULA \
	    --batch Install \
	    --config ${XILINX_INSTALL_LOCATION}/tmp/${XILINX_VIVADO_BATCH_CONFIG_FILE} && \
	rm -rf ${XILINX_INSTALL_LOCATION}/tmp

