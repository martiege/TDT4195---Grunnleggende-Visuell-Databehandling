import numpy as np
import skimage
import utils
import matplotlib.pyplot as plt
from task4b import convolve_im


def sharpen(im: np.array):
    """ Sharpens a grayscale image using the laplacian operator.

    Args:
        im: np.array of shape [H, W]
    Returns:
        im: np.array of shape [H, W]
    """
    laplacian = np.array([
        [0, -1, 0],
        [-1, 4, -1],
        [0, -1, 0]
    ])
    ### START YOUR CODE HERE ### (You can change anything inside this block)
    
    verbose = True
    im += convolve_im(im, laplacian, verbose)

    if verbose:
        plt.savefig("image_processed/moon_sharpened_subplots.png")
    
    ### END YOUR CODE HERE ###
    return im


if __name__ == "__main__":
    # DO NOT CHANGE
    im = skimage.data.moon()
    im = utils.uint8_to_float(im)
    sharpen_im = sharpen(im)

    sharpen_im = utils.to_uint8(sharpen_im)
    im = utils.to_uint8(im)
    # Concatenate the image, such that we get
    # the original on the left side, and the sharpened on the right side
    im = np.concatenate((im, sharpen_im), axis=1)
    utils.save_im("moon_sharpened.png", im)
