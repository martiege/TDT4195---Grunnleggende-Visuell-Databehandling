import numpy as np
import skimage
import utils
import pathlib


def lower_cu_sum(upper_limit, array, element_function)):
    c_sum = 0
    for i in range(0, upper_limit):
        c_sum += element_function(i, array[i])
    
    return c_sum

def upper_cu_sum(lower_limit, array, element_function):
    (k, array, element_function) = args
    (L,) = array.shape
    c_sum = 0
    for i in range(lower_limit, L):
        c_sum += element_function(i, array[i])
    
    return c_sum

def otsu_thresholding(im: np.ndarray) -> int:
    """
        Otsu's thresholding algorithm that segments an image into 1 or 0 (True or False)
        The function takes in a grayscale image and outputs a boolean image

        args:
            im: np.ndarray of shape (H, W) in the range [0, 255] (dtype=np.uint8)
        return:
            (int) the computed thresholding value
    """
    assert im.dtype == np.uint8
    ### START YOUR CODE HERE ### (You can change anything inside this block) 
    # You can also define other helper functions
    # Compute normalized histogram
    (H, W)  = im.shape
    L       = 256

    (hist, _) = np.histogram(im, L, (0, L - 1))
    p = hist / np.sum(hist)
    assert(np.sum(p) == 1)

    P_1 = np.fromiter(
        map(lambda k: lower_cu_sum(k + 1, p, lambda i, p_i: p_i), range(L)), 
        np.float
    )
    P_2 = np.fromiter(
        map(lambda k: upper_cu_sum(k + 1, p, lambda i, p_i: p_i), range(L)), 
        np.float
    )

    m_1 = (1 / P_1) * np.fromiter(
        map(lambda k: lower_cu_sum(k + 1, p, lambda i, p_i: i * p_i), range(L)),
        np.float
    )
    m_2 = (1 / P_2) * np.fromiter(
        map(lambda k: upper_cu_sum(k + 1, p, lambda i, p_i: i * p_i), range(L)),
        np.float
    )
    m   = np.fromiter(
        map(lambda k: lower_cu_sum(k + 1, p, lambda i, p_i: i * p_i), range(L)),
        np.float
    )
    m_G = lower_cu_sum(L - 1, p, lambda i, p_i: i * p_i)

    assert(P_1 * m_1 + P_2 * m_2 == m_G)
    assert(P_1 + P_2 == 1)

    sigma_G2 = lower_cu_sum(L, p, lambda i, p_1: (i - m_G)**2 * p_i)
    sigma_B2 = P_1 * (m_1 - m_G)**2 + P_2 * (m_2 - m_G)**2

    assert(sigma_B2 == P_1 * P_2 * (m_1 - m_2)**2)
    assert(sigma_B2 == ((m_G * P_1 - m)**2 / (P_1 * (1 - P_1))))

    threshold = 128
    return threshold
    ### END YOUR CODE HERE ### 


if __name__ == "__main__":
    # DO NOT CHANGE
    impaths_to_segment = [
        pathlib.Path("thumbprint.png"),
        pathlib.Path("polymercell.png")
    ]
    for impath in impaths_to_segment:
        im = utils.read_image(impath)
        threshold = otsu_thresholding(im)
        print("Found optimal threshold:", threshold)

        # Segment the image by threshold
        segmented_image = (im >= threshold)
        assert im.shape == segmented_image.shape, \
            "Expected image shape ({}) to be same as thresholded image shape ({})".format(
                im.shape, segmented_image.shape)
        assert segmented_image.dtype == np.bool, \
            "Expected thresholded image dtype to be np.bool. Was: {}".format(
                segmented_image.dtype)

        segmented_image = utils.to_uint8(segmented_image)

        save_path = "{}-segmented.png".format(impath.stem)
        utils.save_im(save_path, segmented_image)


