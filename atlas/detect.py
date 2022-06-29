import os
import sys
import math
import operator
import time
from functools import reduce
from PIL import Image


def prepare(pic1: str, pic2: str):
    os.system("bash camera.sh {} > /dev/null 2>&1".format(pic1))
    os.system("bash camera.sh {} > /dev/null 2>&1".format(pic2))


def diff(pic1: str, pic2: str):
    img1 = Image.open(pic1)
    img2 = Image.open(pic2)
    histogram1 = img1.histogram()
    histogram2 = img2.histogram()
    res = math.sqrt(reduce(operator.add, list(
        map(lambda a, b: (a-b)**2, histogram1, histogram2)))/len(histogram1))
    return res


def detect(pic1: str, pic2: str, dir_path: str):
    prepare(pic1, pic2)
    res = diff(pic1, pic2)
    print("diff: " + str(res))
    # 500 允许一定误差
    if res >= 500:
        print("rubbish detected")
        edgex_addr = sys.argv[1]
        device = sys.argv[2]
        valuedescriptor = sys.argv[3]
        img = os.path.join(dir_path, pic2)
        exe = os.path.join(dir_path, "rubbish/classify.py")
        cls_cmd = "python3 {} {}".format(exe, img)
        print("+ " + cls_cmd)
        re = os.popen(cls_cmd).readlines()
        real = re[-2].strip('\n')
        cls = re[-1].strip('\n')
        print("class: {}, name: {}".format(cls, real))
        trans_cmd = "bash d2e.sh {} {} {} {} > /dev/null 2>&1".format(
            cls, edgex_addr, device, valuedescriptor)
        print("+ " + trans_cmd)
        os.system(trans_cmd)
        # 将摄像头休息一会 并避免垃圾拿走时图片变化也触发算法
        print("sleep for a while (3s)")
        time.sleep(3)
        print("restart")


if __name__ == "__main__":
    pic1 = "1.jpg"
    pic2 = "2.jpg"
    dir_path = os.path.realpath(__file__).rsplit("/", 1)[0]
    while True:
        detect(pic1, pic2, dir_path)
        time.sleep(1)
