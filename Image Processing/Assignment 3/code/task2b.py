import utils
import numpy as np

def safe_abs_diff(a, b):
    """
        Computes the difference abs(a - b) safely 
        for unsigned integers by always subtracting from the 
        largest number.

        args:
            a: unsigned integer a (dtype=np.uint8)
            b: unsigned integer b (dtype=np.uint8)
        return: 
            absolute value of the difference 
            between a and b (dtype=np.uint8)
    """
    if a < b: 
        a, b = b, a 
    return a - b


def inside_image(coordinate, H, W):
    """
        The function checks of a given coordinate is inside 
        the region (0,0) to (H, W)

        args: 
            coordinate: tuple of the form (row, col), gives the
                        coordinates to be checked
            H: height of the region
            W: width of the region
        returns: 
            boolean, True if the coordinate is inside the region
    """
    (row, col) = coordinate
    return col < W and col >= 0 and row < H and row >= 0

def generate_neighbourhood(row, col, H, W):
    """
        This function generates Moore-neighbourhood 
        (8-connectedness) which is valid within the region
        (0, 0) to (H, W). 

        args: 
            row: integer, the given row to generate a neighbourhood around
            col: integer, the given column to generate a neighbourhood around
            H: height of the region
            W: width of the region
        returns: 
            list of the valid neighbours of the input
    """
    return list(filter(lambda coordinate: inside_image(coordinate, H, W), 
        [
            (row - 1, col + 1), (row, col + 1), (row + 1, col + 1), 
            (row - 1, col),                     (row + 1, col), 
            (row - 1, col - 1), (row, col - 1), (row + 1, col - 1)
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

