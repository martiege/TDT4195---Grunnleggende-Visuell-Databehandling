import skimage
import os
import numpy as np
import utils
import matplotlib.pyplot as plt
from task4b import convolve_im


if __name__ == "__main__":
    # DO NOT CHANGE

    impath = os.path.join("images", "clown.jpg")
    im = skimage.io.imread(impath)
    im = utils.uint8_to_float(im)
    kernel = np.load("images/notch_filter.npy")

    ### START YOUR CODE HERE ### (You can change anything inside this block)
    verbose = True
    im_filtered = convolve_im(im, kernel, verbose)

    if verbose:
        plt.savefig("image_processed/clown_filtered_subplots.png")

    ### END YOUR CODE HERE ###

    utils.save_im("clown_filtered.png", im_filtered)

