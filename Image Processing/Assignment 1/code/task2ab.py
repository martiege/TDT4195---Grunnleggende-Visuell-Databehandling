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
    # # # # # # # # neato syntax :D
    return np.dot(im[..., :3], [0.212, 0.7152, 0.0722])


def inverse(im):
    """ Finds the inverse of the greyscale image
    
    Args:
        im ([type]): [np.array of shape [H, W]]
    
    Returns:
        im ([type]): [np.array of shape [H, W]]
    """    
     # YOUR CODE HERE
    # simple test for normalization
    if np.max(im) <= 1.0:
        return np.subtract(1.0, im)
    else:
        return np.subtract(255.0, im)


if __name__ == "__main__":
    im = plt.imread("images/lake.jpg")
    im = greyscale(im)
    inverse_im = inverse(im)
    save_im("lake_greyscale.jpg", im, cmap="gray")
    save_im("lake_inverse.jpg", inverse_im, cmap="gray")
