import matplotlib.pyplot as plt
import numpy as np
import skimage
import utils




def convolve_im(im: np.array,
                kernel: np.array,
                verbose=True):
    """ Convolves the image (im) with the spatial kernel (kernel),
        and returns the resulting image.

        "verbose" can be used for visualizing different parts of the 
        convolution.
        
        Note: kernel can be of different shape than im.

    Args:
        im: np.array of shape [H, W]
        kernel: np.array of shape [K, K] 
        verbose: bool
    Returns:
        im: np.array of shape [H, W]
    """
    ### START YOUR CODE HERE ### (You can change anything inside this block)
    H, W = im.shape 
    K, _ = kernel.shape 

    kernel = np.pad(kernel, ((0, H-K), (0, H-K)), 'constant')

    fft_im = np.fft.fft2(im)
    fft_kernel = np.fft.fft2(kernel)
    fft_conv = fft_im * fft_kernel
    conv_result = np.real(np.fft.ifft2(fft_conv))

    if verbose:
        # Use plt.subplot to place two or more images beside eachother
        plt.figure(figsize=(20, 4))
        # plt.subplot(num_rows, num_cols, position (1-indexed))
        plt.subplot(1, 5, 1)
        plt.title("Original image")
        plt.imshow(im, cmap="gray")
        
        plt.subplot(1, 5, 2)
        plt.title("FFT of original image")
        plt.imshow(np.fft.fftshift(np.log(np.abs(fft_im))), cmap="gray")
        
        plt.subplot(1, 5, 3)
        plt.title("FFT kernel")
        plt.imshow(np.fft.fftshift(np.abs(fft_kernel)), cmap="gray")

        plt.subplot(1, 5, 4)
        plt.title("FFT multiplication (convolution)")
        plt.imshow(np.fft.fftshift(np.log(np.abs(fft_conv))), cmap="gray")
        
        plt.subplot(1, 5, 5) 
        plt.title("Resulting image")
        plt.imshow(conv_result, cmap="gray")
    ### END YOUR CODE HERE ###
    return conv_result


if __name__ == "__main__":
    verbose = True # change if you want

    # Changing this code should not be needed
    im = skimage.data.camera()
    im = utils.uint8_to_float(im)

    # DO NOT CHANGE
    gaussian_kernel = np.array([
        [1, 4, 6, 4, 1],
        [4, 16, 24, 16, 4],
        [6, 24, 36, 24, 6],
        [4, 16, 24, 16, 4],
        [1, 4, 6, 4, 1],
    ]) / 256
    image_gaussian = convolve_im(im, gaussian_kernel, verbose)

    if verbose:
        plt.savefig("image_processed/camera_gaussian_subplots.png")

    # DO NOT CHANGE
    sobel_horizontal = np.array([
        [-1, 0, 1],
        [-2, 0, 2],
        [-1, 0, 1]
    ])
    image_sobelx = convolve_im(im, sobel_horizontal, verbose)

    if verbose:
        plt.savefig("image_processed/camera_sobelx_subplots.png")

    if verbose:
        plt.show()

    utils.save_im("camera_gaussian.png", image_gaussian)
    utils.save_im("camera_sobelx.png", image_sobelx)
