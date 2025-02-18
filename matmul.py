import os
import numpy as np
import asyncio

MATRIX_DIM = 8
MATRIX_SIZE = MATRIX_DIM*MATRIX_DIM
DATA_TYPE = np.float32
DATA_SIZE = np.dtype(DATA_TYPE).itemsize

BUFFER_SIZE = MATRIX_SIZE*DATA_SIZE

MAT_A = np.arange(0, MATRIX_SIZE, dtype=DATA_TYPE).reshape(MATRIX_DIM, MATRIX_DIM)
MAT_B = np.arange(0, MATRIX_SIZE, dtype=DATA_TYPE).reshape(MATRIX_DIM, MATRIX_DIM)
MAT_C = MAT_A @ MAT_B

async def to_device():
    xdma_axis_wr_data = os.open('/dev/xdma0_h2c_0', os.O_WRONLY)

    print(f'{MAT_A=}')
    print(f'{MAT_B=}')

    buffer = np.concatenate([MAT_A, MAT_B])
    os.write(xdma_axis_wr_data, buffer.tobytes())

async def from_device():
    xdma_axis_rd_data = os.open('/dev/xdma0_c2h_0', os.O_RDONLY)

    data = os.read(xdma_axis_rd_data, BUFFER_SIZE)
    output = np.frombuffer(data, dtype=DATA_TYPE).reshape(MATRIX_DIM, MATRIX_DIM)
    print(f'{output=}')
    assert np.allclose(MAT_C, output)

async def matmul():
    await to_device()
    await from_device()

asyncio.run(matmul())
