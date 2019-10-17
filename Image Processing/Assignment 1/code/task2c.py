import numpy as np
import os
import matplotlib.pyplot as plt
from time import time
from task2ab import save_im


def convolve_im(im, kernel):
    """ A function that convolves im with kernel
    
    Args:
        im ([type]): [np.array of shape [H, W, 3]]
        kernel ([type]): [np.array of shape [K, K]]
    
    Returns:
        [type]: [np.array of shape [H, W, 3]. should be same as im]
    """
    # YOUR CODE HERE
    (H, W, _)   = im.shape
    (K, _)      = kernel.shape

    assert(K & 1) # odd number
    K_          = K // 2

    zero_padded = np.zeros((H + 2 * K_, W + 2 * K_, 3))
    zero_padded[K_:-K_, K_:-K_, :] = im
    
    # flippedy floppedy I'm taking your property
    flipped_kernel = np.fliplr(np.flipud(kernel))

    for y in range(H):
        for x in range(W):
            for c in range(3):
                product = flipped_kernel * zero_padded[y:y+K, x:x+K, c]
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
    # h_b = np.array((
    #     [1, 0, -1],
    #     [-0, 0, 0],
    #     [-1, 0, 1]
    # ), dtype="float")

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
