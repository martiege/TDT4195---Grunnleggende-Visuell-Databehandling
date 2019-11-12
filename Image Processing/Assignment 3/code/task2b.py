import utils
import numpy as np


def valid_neighbourhood(im, segmented, intensity, x, y, T):
    (H, W) = im.shape

    for x_n in range(x - 1, x + 2):
        for y_n in range(y - 1, y + 2):
            x_check = (x_n < W) and (x_n >= 0)
            y_check = (y_n < H) and (y_n >= 0) and (y_n != y)
            centre  = (x_n == x) and (y_n == y)
            if x_check and y_check and not centre:
                valid_intensity = (np.abs(im[y_n, x_n] - intensity) <= T)
                if not segmented[y_n, x_n] and valid_intensity:
                    segmented[y_n, x_n] = True 
                    valid_neighbourhood(im, segmented, intensity, x_n, y_n, T)


def region_growing(im: np.ndarray, seed_points: list, T: int) -> np.ndarray:
    """
        A region growing algorithm that segments an image into 1 or 0 (True or False).
        Finds candidate pixels with a Moore-neighborhood (8-connectedness). 
        Uses pixel intensity thresholding with the threshold T as the homogeneity criteria.
        The function takes in a grayscale image and outputs a boolean image

        args:
            im: np.ndarray of shape (H, W) in the range [0, 255] (dtype=np.uint8)
            seed_points: list of list containing seed points (row, col). Ex:
                [[row1, col1], [row2, col2], ...]
            T: integer value defining the threshold to used for the homogeneity criteria.
        return:
            (np.ndarray) of shape (H, W). dtype=np.bool
    """
    ### START YOUR CODE HERE ### (You can change anything inside this block)
    # You can also define other helper functions
    segmented = np.zeros_like(im).astype(bool)
    for row, col in seed_points:
        segmented[row, col] = True
        valid_neighbourhood(im, segmented, im[row, col], col, row, T)
    return segmented
    ### END YOUR CODE HERE ### 



if __name__ == "__main__":
    # DO NOT CHANGE
    im = utils.read_image("defective-weld.png")

    seed_points = [ # (row, column)
        [254, 138], # Seed point 1
        [253, 296], # Seed point 2
        [233, 436], # Seed point 3
        [232, 417], # Seed point 4
    ]
    intensity_threshold = 50
    segmented_image = region_growing(im, seed_points, intensity_threshold)

    assert im.shape == segmented_image.shape, \
        "Expected image shape ({}) to be same as thresholded image shape ({})".format(
            im.shape, segmented_image.shape)
    assert segmented_image.dtype == np.bool, \
        "Expected thresholded image dtype to be np.bool. Was: {}".format(
            segmented_image.dtype)

    segmented_image = utils.to_uint8(segmented_image)
    utils.save_im("defective-weld-segmented.png", segmented_image)

