import numpy as np
import os
import matplotlib.pyplot as plt
import scipy.sparse as sparse
from time import time
from task2ab import save_im


# https://ieeexplore.ieee.org/document/7732252

def convolve_im(im, kernel, use_fourier=False):
    """ A function that convolves im with kernel
    
    Args:
        im ([type]): [np.array of shape [H, W, 3]]
        kernel ([type]): [np.array of shape [K, K]]
    
    Returns:
        [type]: [np.array of shape [H, W, 3]. should be same as im]
    """
    # YOUR CODE HERE
    (H, W, C)   = im.shape
    (K, k)      = kernel.shape

    # zero-padding offset
    K_          = K // 2

    assert(K == k) # kernel is of shape [K, K]
    assert(K &  1) # odd number (select 2**0 bit)
    assert(C == 3) # RGB
    # could do some error handling, but not part of the task

    zero_padded = np.zeros((H + 2 * K_, W + 2 * K_, 3))
    zero_padded[K_:-K_, K_:-K_, :] = im
    
    # flippedy floppedy I'm taking your property
    # the kernel is rotated 180 degrees
    # image convolution can be represented as correlation now
    flipped_kernel = np.fliplr(np.flipud(kernel))

    for y in range(H):
        for x in range(W):
            for c in range(C):
                # array multiplication is element-wise
                # correlation can be applied with a flipped kernel
                product = flipped_kernel * zero_padded[y:y+K, x:x+K, c]
                
                # sum of the resulting product is the convolution
                im[y, x, c] = product.sum()

    return im


if __name__ == "__main__":
    # Read image
    im_path = os.path.join("images", "lake.jpg")
    im = plt.imread(im_path)

    # Define the convolutional kernels
    h_a = np.ones((3, 3)) / 9
    h_b = np.array([
        [1,  4,  6,  4, 1],
        [4, 16, 24, 16, 4],
        [6, 24, 36, 24, 6],
        [4, 16, 24, 16, 4],
        [1,  4,  6,  4, 1],
    ]) / 256

    # Convolve images
    t_1_0 = time()
    smoothed_im1 = convolve_im(im.copy(), h_a)
    t_1_1 = time()
    print("Time for h_a: ", t_1_1 - t_1_0)

    t_2_0 = time()
    smoothed_im2 = convolve_im(im.copy(), h_b)
    t_2_1 = time()
    print("Time for h_b: ", t_2_1 - t_2_0)
    print("Total time: ", t_2_1 - t_1_0)

    # DO NOT CHANGE
    assert isinstance(smoothed_im1, np.ndarray), \
        f"Your convolve function has to return a np.array. " +\
        f"Was: {type(smoothed_im1)}"
    assert smoothed_im1.shape == im.shape, \
        f"Expected smoothed im ({smoothed_im1.shape}" + \
        f"to have same shape as im ({im.shape})"
    assert smoothed_im2.shape == im.shape, \
        f"Expected smoothed im ({smoothed_im1.shape}" + \
        f"to have same shape as im ({im.shape})"

    save_im("convolved_im_h_a.jpg", smoothed_im1)
    save_im("convolved_im_h_b.jpg", smoothed_im2)
