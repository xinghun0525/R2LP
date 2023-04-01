# input :（batch，image_generate）（batch，image_refer）
import torch
def Lp_Loss(image_generate, image_refer, p):
    return |abs(image_generate-image_refer)|