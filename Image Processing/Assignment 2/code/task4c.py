import skimage
import os
import numpy as np
import utils
from task4b import convolve_im


if __name__ == "__main__":
    # DO NOT CHANGE

    impath = os.path.join("images", "clown.jpg")
    im = skimage.io.imread(impath)
    im = utils.uint8_to_float(im)
    kernel = np.load("images/notch_filter.npy")

    ### START YOUR CODE HERE ### (You can change anything inside this block)
    im_filtered = im

    ### END YOUR CODE HERE ###

    utils.save_im("clown_filtered.png", im_filtered)

