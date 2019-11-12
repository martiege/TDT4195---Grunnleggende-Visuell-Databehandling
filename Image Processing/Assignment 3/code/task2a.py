import numpy as np
import skimage
import utils
import pathlib


def lower_cu_sum(upper_limit, array, element_function):
    c_sum = 0
    for i in range(0, upper_limit):
        c_sum += element_function(i, array[i])
    
    return c_sum

def upper_cu_sum(lower_limit, array, element_function):
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
    (H, W)  = im.shape
    L       = 256

    # 1. Compute the normalized histogram of the input image. 
    # Denote the components of the histogram by p_i, i = 0, 1, 2, ..., L - 1
    (hist, _) = np.histogram(im, L, (0, L - 1))
    p = hist / np.sum(hist)
    assert(np.sum(p) == 1)

    # 2. Compute the cumulative sums, P_1(k), for k = 0, 1, 2, ..., L - 1
    # using Eq. (10-49)
    P_1 = np.fromiter(
        map(lambda k: lower_cu_sum(k + 1, p, lambda i, p_i: p_i), range(L)), 
        np.float
    )
    # P_2 = np.fromiter(
    #     map(lambda k: upper_cu_sum(k + 1, p, lambda i, p_i: p_i), range(L)), 
    #     np.float
    # )

    # m_1 = np.divide(1, P_1, out=0, where=P_1!=0) * np.fromiter(
    #     map(lambda k: lower_cu_sum(k + 1, p, lambda i, p_i: i * p_i), range(L)),
    #     np.float
    # )
    # m_2 = np.divide(1, P_2, out=0, where=P_2!=0) * np.fromiter(
    #     map(lambda k: upper_cu_sum(k + 1, p, lambda i, p_i: i * p_i), range(L)),
    #     np.float
    # )

    # 3. Compute the cumulative means, m(k), for k = 0, 1, 2, ..., L - 1, 
    # using Eq. (10-53)
    m   = np.fromiter(
        map(lambda k: lower_cu_sum(k + 1, p, lambda i, p_i: i * p_i), range(L)),
        np.float
    )
    # 4. Compute the global mean, m_G, using Eq. (10-54)
    m_G = lower_cu_sum(L - 1, p, lambda i, p_i: i * p_i)

    # assert((P_1 * m_1 + P_2 * m_2 == m_G).any())
    # assert(P_1 + P_2 == 1)

    # 5. Compute the between-class variance term, sigma_B2(k), for k = 0, 1, 2, ..., L - 1
    # using Eq. (10-62)
    sigma_B2 = (m_G * P_1 - m)**2 / (P_1 * (1 - P_1))

    # assert(sigma_B2 == P_1 * P_2 * (m_1 - m_2)**2)
    # assert(sigma_B2 == ((m_G * P_1 - m)**2 / (P_1 * (1 - P_1))))

    # 6. Obtain the Otsu threshold, k_star, as the value of k for which sigma_B2(k) is maximum.
    # If the maximum is not unique, obtain k_star by averaging the values of k corresponding to 
    # the various maxima detected.
    k_star_max = np.amax(sigma_B2)
    k_star_lst = np.argwhere(sigma_B2 == k_star_max).flatten().tolist()
    k_star = np.floor_divide(np.sum(k_star_lst), len(k_star_lst))

    print(k_star)

    # 7. Compute the global variance, sigma_G2, using Eq. (10-58), and then obtain the separability 
    # measure, eta_star, by evaluating Eq. (10-61) with k = k_star
    sigma_G2 = lower_cu_sum(L, p, lambda i, p_i: (i - m_G)**2 * p_i)
    eta = sigma_B2 / sigma_G2
    print(eta)
    print(eta.shape)
    # eta_star = eta[k_star]

    threshold = k_star
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


