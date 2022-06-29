from atlas_utils.acl_resource import AclResource
from atlas_utils.acl_image import AclImage
from atlas_utils.acl_model import Model
from atlas_utils.acl_dvpp import Dvpp
import sys
import os

SRC_PATH = os.path.realpath(__file__).rsplit("/", 1)[0]
MODEL_PATH = os.path.join(SRC_PATH, "./garbage_yuv.om")
MODEL_WIDTH = 224
MODEL_HEIGHT = 224

garbage_classes = {
    '干垃圾': ['贝壳', '打火机', '旧镜子', '扫把', '陶瓷碗', '牙刷', '一次性筷子', '脏污衣服'],
    '可回收物': ['报纸', '玻璃制品', '篮球', '塑料瓶', '硬纸板', '玻璃瓶', '金属制品', '帽子', '易拉罐', '纸张'],
    '湿垃圾': ['菜叶', '橙皮', '蛋壳', '香蕉皮'],
    '有害垃圾': ['电池', '药片胶囊', '荧光灯', '油漆桶'],
    '其他垃圾': ['其他']
}


def get_cls(class_id):
    all = 0
    cur = -1
    for key, values in garbage_classes.items():
        cur += 1
        idx = class_id - all
        all += len(values)
        if len(values) == 1:
            return cur, values[0]
        if class_id >= all:
            continue
        return cur, values[idx]


def prepare(image, dvpp):
    """pre process"""
    image_input = image.copy_to_dvpp()
    yuv_image = dvpp.jpegd(image_input)
    # decode jpeg end
    resized_image = dvpp.resize(yuv_image, MODEL_WIDTH, MODEL_HEIGHT)
    # resize yuv end
    return resized_image


def classify(img: str):
    acl_resource = AclResource()
    acl_resource.init()
    model = Model(MODEL_PATH)
    dvpp = Dvpp(acl_resource)
    image = AclImage(img)
    resized_image = prepare(image, dvpp)
    result = model.execute([resized_image, ])
    data = result[0]
    vals = data.flatten()
    top_k = vals.argsort()[-1:-6:-1]
    return top_k[0]


if __name__ == "__main__":
    img = sys.argv[1]
    result = get_cls(classify(img))
    print(result[1])
    print(result[0])
