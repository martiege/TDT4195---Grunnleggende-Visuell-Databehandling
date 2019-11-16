import utils
import numpy as np

# Seed point: (233, 436)
# Seed point intensity: 255
# Position row: 239, col: 453: Image intensity: 255
# Position row: 239, col: 454: Image intensity: 252

# Segmented Position row: 239, col: 453: Segmented Value: True
# Segmented Position row: 239, col: 454: Segmented Value: False

# Your segmented image in position (239, 454) should be True, but it is False.

# I recommend you to debug your code for this special case.

def valid_neighbourhood(im, segmented, intensity, x, y, T, verbose=False):
    (H, W) = im.shape

    for x_n, y_n in generate_neighbourhood(x, y, H, W):
        valid_intensity = safe_abs_diff(intensity, im[y_n, x_n]) <= T

        if verbose:
            print("Column:", y_n, "Row:", x_n)
            print("This intensity:", im[y_n, x_n], "Seed intensity:", intensity)
            print("Difference:", intensity - im[y_n, x_n], "Abs: ", np.abs(intensity - im[y_n, x_n]))
            print("T: ", T, "Less than T:", np.abs(intensity - im[y_n, x_n]) <= T, "Valid:", valid_intensity)
            print("")

        if not segmented[y_n, x_n] and valid_intensity:
            segmented[y_n, x_n] = True 
            valid_neighbourhood(im, segmented, intensity, x_n, y_n, T, verbose=verbose)

def safe_abs_diff(uint_a, uint_b):
    """
        Computes the difference abs(uint_a - uint_b) safely 
        for unsigned integers by always subtracting from the 
        largest number.
    """
    if uint_a < uint_b: 
        uint_a, uint_b = uint_b, uint_a 
    return uint_a - uint_b


def inside_image(c, H, W):
    return c[0] < W and c[0] >= 0 and c[1] < H and c[1] >= 0

def generate_neighbourhood(x, y, H, W):
    return list(filter(lambda coordinate: inside_image(coordinate, H, W), 
        [
            (x - 1, y + 1), (x, y + 1), (x + 1, y + 1), 
            (x - 1, y),                 (x + 1, y), 
            (x - 1, y - 1), (x, y - 1), (x + 1, y - 1)
        ]
    ))

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
    (H, W) = im.shape

    segmented = np.zeros_like(im).astype(bool)
    for row_seed, col_seed in seed_points:
        active = [(row_seed, col_seed)]
        seed_intensity = im[row_seed, col_seed]
        while len(active) > 0:
            row, col = active.pop(0)
            if not segmented[row, col] and safe_abs_diff(seed_intensity, im[row, col]) <= T:
                segmented[row, col] = True
                active += generate_neighbourhood(row, col, H, W)
                
        # tried recursion, didn't completely fill the proper regions
        # any feedback on what went wrong?
        # valid_neighbourhood(im, segmented, im[row, col], col, row, T, row == 233 and col == 436)
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

