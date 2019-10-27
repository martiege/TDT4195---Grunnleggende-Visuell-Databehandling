import matplotlib.pyplot as plt
import numpy as np
import os

image_output_dir = "image_processed"
os.makedirs(image_output_dir, exist_ok=True)


def save_im(imname, im, cmap=None):
    impath = os.path.join(image_output_dir, imname)
    plt.imsave(impath, im, cmap=cmap)


def greyscale(im):
    """ Converts an RGB image to greyscale
    
    Args:
        im ([type]): [np.array of shape [H, W, 3]]
    
    Returns:
        im ([type]): [np.array of shape [H, W]]
    """
    # YOUR CODE HERE
    # using dot function from numpy
    # the weighted average is applied to the RGB colours
    return np.dot(im[..., :3], [0.212, 0.7152, 0.0722])


def inverse(im):
    """ Finds the inverse of the greyscale image
    
    Args:
        im ([type]): [np.array of shape [H, W]]
            type is in the range [0, 255]
    
    Returns:
        im ([type]): [np.array of shape [H, W]]
            type is in the range [0, 255]
    """    
     # YOUR CODE HERE
    # note range of type is important here 
    # image pixel value is assumed to be between 0 and 255
    return np.subtract(255.0, im)


if __name__ == "__main__":
    im = plt.imread("images/lake.jpg")
    im = greyscale(im)
    inverse_im = inverse(im)
    save_im("lake_greyscale.jpg", im, cmap="gray")
    save_im("lake_inverse.jpg", inverse_im, cmap="gray")
