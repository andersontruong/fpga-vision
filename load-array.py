#!/usr/bin/env python3
import os
import time
import numpy as np
import sys
import argparse

DEV = "/dev/xdma0_user"


def mmap(base_addr, length):
    import mmap
    euid = os.geteuid()
    if euid != 0:
        raise EnvironmentError('Root permissions required.')

    # Align the base address with the pages
    virt_base = base_addr & ~(mmap.PAGESIZE - 1)

    # Calculate base address offset w.r.t the base address
    virt_offset = base_addr - virt_base

    # print(length, virt_offset, virt_base)
    #
    # print(dir(mmap))

    # Open file and mmap
    mmap_file = os.open(DEV, os.O_RDWR | os.O_SYNC)
    mem = mmap.mmap(mmap_file, length + virt_offset,
                    mmap.MAP_ANONYMOUS | mmap.MAP_SHARED,
                    mmap.PROT_READ | mmap.PROT_WRITE,
                    offset=virt_base)
    os.close(mmap_file)
    array = np.frombuffer(mem, np.uint32, length >> 2, virt_offset)
    return array

class MMIO:
    def __init__(self, base_addr, length=4, **kwargs):
        if 'debug' in kwargs:
            warnings.warn("Keyword debug has been deprecated.",
                          DeprecationWarning)

        if base_addr < 0 or length < 0:
            raise ValueError("Base address or length cannot be negative.")

        self.base_addr = base_addr
        self.length = length

        self.read = self.read
        self.write = self.write_mm
        self.array = mmap(base_addr, length)

    def read(self, offset=0, length=4, word_order='little'):
        if length not in [1, 2, 4, 8]:
            raise ValueError("MMIO currently only supports "
                             "1, 2, 4 and 8-byte reads.")
        if offset < 0:
            raise ValueError("Offset cannot be negative.")
        if length == 8 and word_order not in ['big', 'little']:
            raise ValueError("MMIO only supports big and little endian.")
        idx = offset >> 2
        if offset % 4:
            raise MemoryError('Unaligned read: offset must be multiple of 4.')

        # Read data out
        lsb = int(self.array[idx])
        if length == 8:
            if word_order == 'little':
                return ((int(self.array[idx + 1])) << 32) + lsb
            else:
                return (lsb << 32) + int(self.array[idx + 1])
        else:
            return lsb & ((2 ** (8 * length)) - 1)

    def write_mm(self, offset, data):
        if offset < 0:
            raise ValueError("Offset cannot be negative.")

        idx = offset >> 2
        if offset % 4:
            raise MemoryError('Unaligned write: offset must be multiple of 4.')

        if type(data) is int:
            self.array[idx] = np.uint32(data)
        elif type(data) is bytes:
            length = len(data)
            num_words = length >> 2
            if length % 4:
                raise MemoryError(
                    'Unaligned write: data length must be multiple of 4.')
            buf = np.frombuffer(data, np.uint32, num_words, 0)
            for i in range(len(buf)):
                self.array[idx + i] = buf[i]
        else:
            raise ValueError("Data type must be int or bytes.")
  
def mem_test_random():
    data = bytearray()
    for i in range(10*10):
        num = 1
        data.extend(num.to_bytes(4, byteorder='little'))

    fd_h2c = os.open("/dev/xdma0_h2c_0", os.O_WRONLY)
    fd_c2h = os.open("/dev/xdma0_c2h_0", os.O_RDONLY)
    os.pwrite(fd_h2c, data, 0);

    ip = MMIO(0x0002_0000, 0x10000)

    ip.write(0x00, 1)

    # for i in range(101):
    #     data = os.pread(fd_c2h, 4, 4*i)
    #     print(data)

    os.close(fd_h2c)
    os.close(fd_c2h)

    return True

##############################################    

if __name__ == '__main__':

    # status
    exitcode = 0 # default to ok

    parser = argparse.ArgumentParser(description ='litefury/nitefury memory test over pcie')

    parser.add_argument('-s', '--size',
                    default=256,
                    dest='size_mib',
                    help='memory size in mib',
                    type=int
                    )

    parser.add_argument('-i', '--iterations',
                    default=1,
                    dest='test_iters',
                    help='number of times to write/read memory',
                    type=int
                    )


    args = parser.parse_args()

    # clean error log
    with open('ddrtest-err.log', 'w') as errlog:
        errlog.write("starting test. size=%d\n" % (args.size_mib))

    # run test
    # for iter in range(args.test_iters):
    #     passed = mem_test_random(args.size_mib)
    #
    #     if passed: print ("ddr test passed")
    #     else: 
    #         exitcode = 1
#         print ("! ddr test failed")
#         with open('ddrtest-err.log', 'a') as errlog:
#            errlog.write("iteration %d failed\n" % (iter))

# run test to load 10x10 array
    passed = mem_test_random()
    if passed: 
        print("ddr test passed")
    else:
        exitcode = 1
        print("! ddr test failed")
        with open('ddrtest-err.log', 'a') as errlog:
            errlog.write("Iteration %d failed\n" % (iter))

    sys.exit(exitcode)
