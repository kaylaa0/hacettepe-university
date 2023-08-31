# BBM418 Assignment 2
# Image Panorama Stitching
# Kayla Akyüz
# 21726914

import cv2
import numpy as np
import os
import glob
import matplotlib.pyplot as plt
import time
from PIL import Image

# ----------------
# Image operations
# ----------------
def save_image(img, name="Test"): # A function to save images to disk, takes parameter img data and name
    img = Image.fromarray(img) # Convert array to image
    path = "image_out/" + name + ".png" # Creating the full path
    dir_path = os.path.dirname(path) # Getting all folders that needed
    if not os.path.exists(dir_path): # If folders are not existing
        os.makedirs(dir_path) # Creating necessary folders
    img.save(path, format="png") # Save to the disk

def display_image(image, title="", max_size=(10, 10), dpi=300): # A function to ease displaying image with matplotlib
    plt.figure(figsize=max_size, dpi=dpi) # Set the max size of the image
    plt.imshow(image) # Display the image
    plt.title(title) # Set the title
    plt.axis("off") # Remove the axis
    plt.show() # Show the image

# ---------------
# File Operations
# ---------------
def read_ground_truth_homographies(dataset_path): # A function to read provided ground truth homohraphies
    H_files = sorted(glob.glob(os.path.join(dataset_path, "H_*"))) # Filtering file names to get a list
    ground_truth_homographies = [] # Initializing an empty array
    
    for filename in H_files: # Creating a loop to read and add files
        H = np.loadtxt(filename) # Reading file
        ground_truth_homographies.append(H) # Adding to array
    
    return ground_truth_homographies # Returning result array

def read_images(dataset_path): # A function to read images with in the folder
    filenames = sorted(glob.glob(os.path.join(dataset_path, "*.png"))) # Filtering image names to get a list
    sub_images = [] # Initializing an empty array

    for filename in filenames: # Creating a loop to read and add images
        img = cv2.imread(filename, cv2.IMREAD_COLOR) # Load images as color
        sub_images.append(img) # Adding image to array
    sub_images = [cv2.cvtColor(image, cv2.COLOR_BGR2RGB) for image in sub_images]
    return sub_images # Returning result array

def get_folder_names(dataset_path): # Gets sub folder names from the main folder, used to iterate on all images easly
    folder_names = [folder for folder in os.listdir(dataset_path) if os.path.isdir(os.path.join(dataset_path, folder))] # Filtering the folder names
    return folder_names # Returning result array

# -------------------
# Printing Operations
# -------------------
def printout_feature_points(sub_images, keypoints, method, dpi = 100): # A function to print out feature points
    for idx, (image, kp) in enumerate(zip(sub_images, keypoints)): # Enumerating the given data
        feature_plot = cv2.drawKeypoints(image, kp, None) # Drawing the keypoints
        display_image(feature_plot, f"Feature Points - {method} - Image {idx}", dpi = dpi) # Displaying images

def printout_feature_matches(sub_images, keypoints, filtered_matches, method, dpi = 100): # A function to print out feature matches
    for i, match in enumerate(filtered_matches): # Enumerating the given data
        matching_plot = cv2.drawMatches(sub_images[0], keypoints[0], sub_images[i + 1], keypoints[i + 1], match, None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS) # Drawing lines
        display_image(matching_plot, f"Feature Point Matching - {method} - Image 0 - Image {i + 1}", dpi = dpi) # Displaying images

def printout_warped_images(warped_images, method, dpi = 100): # A function to display warped images, even this was not required I found it usefull
    for idx, image in enumerate(warped_images): # Enumerating the given data
        display_image(image, f"Warped Image - {method} - Image {idx}", dpi = dpi) # Displaying images

def printout_method_comparison(images, times, titles): # A function to create a table of comparison for the methods
    rows = len(images) # Setting rows to image count
    cols = 2
    fig, ax = plt.subplots(rows, cols, figsize=(15, rows * 5), dpi=300) # Creating the table

    for i in range(rows): 
        ax[i, 0].imshow(images[i]) # Display panorama image
        ax[i, 0].set_title(titles[i])
        ax[i, 0].axis("off")
        
        ax[i, 1].barh(titles[i], times[i], 0.5, color="purple") # Display runtime as a bar plot
        ax[i, 1].set_xlabel("Runtime (s)")
        ax[i, 1].set_xlim(0, max(times))

    plt.tight_layout()
    plt.show() # Showing the plot

# --------------------------
# Part 1: Feature Extraction
# --------------------------
def feature_extraction(sub_images, method="SIFT"): # A modular way to get opencv function which is allowed to be used at this part
    if method == "SIFT":
        keypoint_extractor = cv2.xfeatures2d.SIFT_create() # Make sure you are using correct opencv version, this lines are the most crucial and are prone to errors
    elif method == "SURF":
        keypoint_extractor = cv2.xfeatures2d.SURF_create()
    elif method == "ORB":
        keypoint_extractor = cv2.ORB_create(nfeatures=100000, edgeThreshold=80, scaleFactor=1.1, nlevels = 10)

    keypoints = [] # Initializing arrays
    descriptors = []

    for sub_image in sub_images:
        keypoint, descriptor = keypoint_extractor.detectAndCompute(sub_image, None) # Using opencv builtin function to extract
        keypoints.append(keypoint) # Adding results to array
        descriptors.append(descriptor)

    return keypoints, descriptors # Returning computed arrays

# ------------------------
# Part 2: Feature Matching
# ------------------------
def feature_matching(descriptors, matcher_type="BF"): # Feature matching function using opencv as it was allowed in this part
    if matcher_type == "BF":
        matcher = cv2.BFMatcher() # Using BF Matcher
    if matcher_type == "BFHAM":
        matcher = cv2.BFMatcher(normType = cv2.NORM_HAMMING) # Norm Hamming used for ORB, it gives way better results

    matches = []
    for i in range(1, len(descriptors)):
        match = matcher.knnMatch(descriptors[0], descriptors[i], k=2) # Getting knn matches
        matches.append(match) # Adding results to array
        
    return matches # Returning computed arrays

def filter_matches(matches, ratio_thres=0.7): # There are too many matches with some methods and it causes performance problems as well as quality degrade
    filtered_matches = [] # In this function the mathes are truncated
    for match in matches: # Looping all matches
        good_match = [] # Creating an array to hold good matches
        for m, n in match: # For m,n that are in the match
            if m.distance < ratio_thres * n.distance: # Calculating distances and checking if they are within threshold
                good_match.append(m) # If so adding to the array
        filtered_matches.append(good_match)
        
    return filtered_matches # Returing result arrays

# --------------------------
# Part 3: Finding Homography
# --------------------------
def iterate_method(method, sub_images, referance_is_0 = True, ransac = 2000, non_filtered = False, matcher="BF", filter_ratio=0.7, verbose=True): # A function to call all necessary operations and return homographies with measure runtime
    start_time = time.time() # Starting timer

    
    keypoints, descriptors = feature_extraction(sub_images, method=method) # Extracing features
    matches = feature_matching(descriptors, matcher) # Computing matches
    if not non_filtered: # Not filtering ORB since it already has low matches
        filtered_matches = filter_matches(matches, ratio_thres=filter_ratio) # Filtering some of the matches as they degrade performance and quality
    else:
        filtered_matches = filter_matches(matches, ratio_thres=1.0)
    homographies = find_homography(keypoints, filtered_matches, referance_is_0, ransac) # Calculating Homographies
    end_time = time.time() # Stopping timer
    runtime = end_time - start_time # Calculating runtime

    if verbose: # This is to be able to skip prints as printing slows down the program, the runtime is returned anyways to construct comperasion table
        print(f"Method: {method} - Runtime: {runtime:.2f} seconds") # Printing out the result

    return homographies, runtime, keypoints, filtered_matches # Returning calcuated data

def find_homography(keypoints, filtered_matches, referance_is_0=True, ransac = 2000): # Main function to compute homographies given keypoints and matches
    homographies = []
    
    for i, matches in enumerate(filtered_matches): # looping over the matches
        if referance_is_0: # Since image 0 is referance and since with in the given dataset the pairs were 0-1, 0-2 etc. I also calculated homographies between referance image and sub-images
            src_pts = np.float32([keypoints[0][m.queryIdx].pt for m in matches]).reshape(-1, 1, 2) # Getting source points 
        else: # However still there is the method to calculate between pairs 1-2, 2-3 etc. as stated in paper
            src_pts = np.float32([keypoints[i][m.queryIdx].pt for m in matches]).reshape(-1, 1, 2)
        dst_pts = np.float32([keypoints[i + 1][m.trainIdx].pt for m in matches]).reshape(-1, 1, 2)
        
        H = ransac_homography(src_pts, dst_pts, iterations = ransac) # Calculating ransac homography on points
        
        if H is not None: # Checking if homography calcution is successful, this was used a lot while developing, hopefully it won't get triggered
            H = H.astype(np.float32) 
            homographies.append(H) # Adding computed homography to array
        else:
            if referance_is_0:
                print(f"Warning: Homography computation failed for image pair (0, {i + 1}). Skipping.")
            else:
                print(f"Warning: Homography computation failed for image pair ({i}, {i + 1}). Skipping.")
            continue
        
    return homographies # Returning calculated homographies

def ransac_homography(src_pts, dst_pts, iterations=2000, threshold=3): # A function to calculate ransac homography
    best_inlier_count = 0 # Initializing best inliner and best homography as none
    best_homography = None

    if len(src_pts) != len(dst_pts) or len(src_pts) < 4: # Checking point amount as it needs to be minimum 4
        raise ValueError("The number of source and destination points must be equal and at least 4.")

    src_pts = np.array(src_pts) # Converting point arrays to NumPy arrays
    dst_pts = np.array(dst_pts)

    for _ in range(iterations): # Looping as much as iterations, I figured 2000 was sufficient and fast enough
        indices = np.random.choice(len(src_pts), 4, replace=False) # Randomly selecting 4 indices from the number of source points without replacement
        src_subset = src_pts[indices].reshape(-1, 2) # Getting the corresponding points using indices
        dst_subset = dst_pts[indices].reshape(-1, 2)

        homography = compute_homography_matrix(src_subset, dst_subset) # Computing homography matrix using the selected point subsets
        if homography is None: # Skipping the iteration if the homography matrix is None
            continue

        inliers = 0 # Initializing the inlier count to 0
        for i in range(len(src_pts)): # Looping through all source points and checking if they are inliers based on the computed homography
            projected_point = np.dot(homography, np.append(src_pts[i], 1)) # Projecting the source point using the computed homography
            if np.abs(projected_point[-1]) > 1e-6:
                projected_point = projected_point / projected_point[-1] # Normalizing the projected point if the last value is non-zero
            else:
                continue

            distance = np.linalg.norm(projected_point[:2] - dst_pts[i]) # Calculating the Euclidean distance between the projected point and the corresponding destination point

            if distance < threshold:
                inliers += 1 # Incrementing the inlier count if the distance is below the given threshold

        if inliers > best_inlier_count: # Checking if the current inlier count is greater than the previous best
            best_inlier_count = inliers # Updating the best inlier count and best homography 
            best_homography = homography

    if best_homography is None: # Checking if the homography calculated succesfully 
        raise RuntimeError("Failed to find a valid homography matrix.")

    return best_homography

def compute_homography_matrix(src_pts, dst_pts): # A function to computing the homography matrix from the given points
    def normalize_points(pts): # A function to normalize points
        pts_homogeneous = np.hstack((pts, np.ones((pts.shape[0], 1)))) # Appending ones to the points to make them homogeneous
        centroid = np.mean(pts, axis=0) # Calculating the centroid of the points
        scale = np.sqrt(2) / np.mean(np.linalg.norm(pts - centroid, axis=1)) # Calculating the scaling factor, such that the mean distance from the points to the centroid is sqrt(2)
        T = np.array([[scale, 0, -scale * centroid[0]], [0, scale, -scale * centroid[1]], [0, 0, 1]]) # Constructing the normalization matrix
        normalized_pts = (T @ pts_homogeneous.T).T # Normalizing the points using the normalization matrix
        return normalized_pts[:, :2], T

    src_pts_normalized, T1 = normalize_points(src_pts) # Normalizing source points
    dst_pts_normalized, T2 = normalize_points(dst_pts) # Normalizing destination points

    A = []
    for p1, p2 in zip(src_pts_normalized, dst_pts_normalized):# Looping through the normalized source and destination points
        x1, y1 = p1
        x2, y2 = p2
        A.append([0, 0, 0, -x1, -y1, -1, y2 * x1, y2 * y1, y2]) # Creating equation rows based on the points and appending them to the A matrix
        A.append([x1, y1, 1, 0, 0, 0, -x2 * x1, -x2 * y1, -x2])

    A = np.array(A)
    try:
        _, _, VT = np.linalg.svd(A) # Calculating the singular value decomposition
    except np.linalg.LinAlgError:
        return None # Returning None if SVD fails

    h = VT[-1] # Getting the last row of the VT matrix as the homographic vector

    H_normalized = h.reshape(3, 3) # Reshaping homographic vector to a 3x3 matrix to obtain the normalized homography
    H = np.linalg.inv(T2) @ H_normalized @ T1 # Denormalizing the homography matrix using the normalization matrices
    
    if np.abs(H[-1, -1]) > 1e-6:
        H = H / H[-1, -1] # Normalizing the homography matrix if the last element is non-zero
    else:
        return None # Returning None if the last element is zero

    return H # Returning the computed homography matrix

# ---------------------------------
# Part 4: Merging by Transformation
# ---------------------------------
def get_warped_image_bounds(img, H): # A function to apply homography on the imagee cornes (width, height) so that the warped image size can be estimated
    h, w = img.shape[0], img.shape[1] # Getting height and with from image shape
    test = np.array([[0, w, w, 0], # Constructing the vectorized corners
                     [0, 0, h, h],
                     [1, 1, 1, 1]])
    test_c = np.dot(np.linalg.inv(H), test) # Applying homography to corners
    test_w = test_c[0] / test_c[2] # Normalizing resulting points
    test_h = test_c[1] / test_c[2]
    
    # Below is a sophisticated line that computes the edges in a smart way
    # In my warp function I wanted to show as much as data as I can so the part of the image that sits on the negative side of the canvas can also be seen
    # In order to that I needed to get the leftmost corner, however the leftmost corner might be positive
    # Worse, left most corner can be so left (this happens with fifth image of bird dataset) that it integer overflows to the right side
    # So that there is a glitched part of the image sticking from the right edge
    # I will put example pictures in the report
    # Below line does its best, while trying to not getting too complicated
    # It checks if our image is one of the peciluar cases
    # If so, lets say the leftmost is positive it simply sets it as 0
    # If leftmost is integer overflow, it sets it as -5000, as this is more than enough, the image gets glitched a lot anyways
    # Also it crops the sticking out part from the right edge by setting right edge to warped max of warped right edges
    # What it does not do is applying same stuff to top, down, or right to left etc.
    # It would get too complicated and I wanted to keep this as a proof of concept and skill :)
    max_y, min_y = test_w[1:2].max() if ((test_w.min() == test_w[1:2].min()) and (test_w.min() > 0)) else test_w.max(), test_w.min() if test_w.min() <= 0 else 0 if test_w.min() != test_w[1:2].min() else -5000
    max_x, min_x = test_h.max(), min(0, test_h.min())
    
    return min_x, min_y, max_x, max_y # Returning the values

#       ----------------------------------
#       Sub-Part A: Applying Homography Matrix
#       ----------------------------------
def warp_perspective(img, H, reverse=True): # The function to warp images by applying reversed homography matrix
    min_x, min_y, max_x, max_y = get_warped_image_bounds(img, H) # Trying to predict the shape of the warped image in a smart way
    w, h = int(max_y - min_y), int(max_x - min_x) # Calculating width and height from the x and y

    if reverse:
        H = np.linalg.inv(H) # Reversing the homography

    # The general explanation is this:
    # Instead of applying homography to image which can result in glitched image, for example empty pixels
    # After the warped image shape is estimated, we apply the homography in reverse and use the image as a lookup table
    # This way no pixel will be empty or glitched, all pixels will get a value
    target_y, target_x = np.meshgrid(np.arange(min_x, max_x), np.arange(min_y, max_y)) # Creating coordinate matrices that represents a canvas for the warped image
    target_coordinates = np.stack([target_x.ravel(), target_y.ravel(), np.ones(target_x.size)]) # Vectorizing the coordinate matrix, using vectorized solution to compute is way faster
    source_coordinates = np.dot(np.linalg.inv(H), target_coordinates) # Applying the reversed homography to target coordinates to get where they corespond in source image
    source_coordinates /= source_coordinates[2, :] # Normalizing the coordinates

    valid = np.logical_and(np.logical_and(0 <= source_coordinates[0, :], source_coordinates[0, :] < img.shape[1]),
                            np.logical_and(0 <= source_coordinates[1, :], source_coordinates[1, :] < img.shape[0])) # Creating a mask from the valid coordinates

    valid_source_coordinates = source_coordinates[:, valid].astype(int) # Applying mask to coordinate systems to get image cutouts
    valid_target_coordinates = target_coordinates[:, valid].astype(int)

    valid_source_coordinates[0] = np.clip(valid_source_coordinates[0], -np.inf, img.shape[1] - 1) # Clipping the coordinates to the image shape
    valid_source_coordinates[1] = np.clip(valid_source_coordinates[1], -np.inf, img.shape[0] - 1)
    
     # This is another part where the fine adjustment is made
     # After applying homography matrix, the point of interest aligns to the left edge
     # However there is data in the negative region
     # In order to make all data visible, the image is transformed to the right and bottom as much as the calculated negative minimum corner
     # In the future (merging) this transform is substracted to align images back
     # However while developing or printing out warped images this transform allows all of the image to be warped
    valid_target_coordinates[0] += int(abs(min_y))
    valid_target_coordinates[1] += int(abs(min_x))
    valid_target_coordinates[0] = np.clip(valid_target_coordinates[0], -np.inf, w - 1) # Clipping target coordinates
    valid_target_coordinates[1] = np.clip(valid_target_coordinates[1], -np.inf, h - 1)

    warped_image = np.zeros((h, w, 3), dtype=np.uint8) # Creating an array to hold warped image
    for i in range(3):
        warped_image[:,:, i][valid_target_coordinates[1], valid_target_coordinates[0]] = img[:,:, i][valid_source_coordinates[1], valid_source_coordinates[0]] # Replacing the pixels from the source image

    return warped_image # Returning warped image

#       ---------------------------------------
#       Sub-Part B: Constructing The Panorama Image
#       ---------------------------------------
def image_merging(images, homographies, alpha = False): # The function to merge images, it allows experimental alpha merging, where I thought how the panorama would look if images were transparent. Normaly the latest image override the one on the bottom and the images on the bottom can be seen only if their parts are sticking out
    if len(images) - 1 != len(homographies): # Checking if there are miscomputed homographies
        raise ValueError("The number of homography matrices must be one less than the number of images.")
    
    warped_images = [images[0]] + [warp_perspective(img, H, True) for img, H in zip(images[1:], homographies)] # Warp all images except the first one using their corresponding homography matrices
    warped_image_bounds = np.array([(0, 0, *images[0].shape[:2])] + [get_warped_image_bounds(img, H) for img, H in zip(images[1:], homographies)]) # Calculate the bounds of each warped image

    min_x = min(-warped_image_bounds[:,0].max(), warped_image_bounds[:,0].min()) # Compute the size of the merged image
    min_y = min(-warped_image_bounds[:,1].max(), warped_image_bounds[:,1].min()) # Making sure there is more than enough canvas to place images, as some images are warped in extreme ways, having more than enough is necessary
    max_x = warped_image_bounds[:,2].max()
    max_y = warped_image_bounds[:,3].max()

    merged_width = int(np.ceil(max_y - min_y - min_y)) # Calculating panorama image size
    merged_height = int(np.ceil(max_x - min_x - min_x))

    # Initialize the merged image with zeros and a count matrix to keep track of the number of images contributing to each pixel
    # This way the pixel values can be summed and after the merging is done each pixel can be divided by how many overlaps did occur
    # So the result panorama image will be as if the images are added transparent
    merged_image = np.zeros((merged_height, merged_width, 3), dtype=np.float32)
    if alpha:
        count_matrix = np.zeros((merged_height, merged_width), dtype=np.uint8)

    for img, bounds in zip(warped_images, warped_image_bounds): # Looping the images
        yy, xx = bounds[:2] # Getting the warped image offset, there was offset to view all of the warped image however it will be discarded to align

        mask = np.all(img == 0, axis=-1) # Getting mask from valid pixels
        y_offset, x_offset = int(max(0, -min_x + yy)), int(max(0, -min_y + xx)) # Substracting the image's offset to align and adding the biggest offset so that the panorama image will also have negative regions visible
        merged_image_slice = merged_image[y_offset:y_offset + img.shape[0], x_offset:x_offset + img.shape[1]] # Applying offset to get a slice from merged image 

        if alpha:
            merged_image_slice[~mask] += img[~mask] # Updating the merged image, summing if we are going for alpha method
        else:
            merged_image_slice[~mask] = img[~mask] # Replacing if we are doing the normal way

        if alpha:
            count_matrix_slice = count_matrix[y_offset:y_offset + img.shape[0], x_offset:x_offset + img.shape[1]] # Getting offset region of count matrix
            count_matrix_slice[~mask] += 1 # Updating the count matrix

    if alpha:
        merged_image /= np.maximum(count_matrix[..., np.newaxis], 1) # If the alpha method is done the pixels have summed up value. Normalizing the merged image by dividing by the count matrix

    return merged_image.astype(np.uint8), warped_images # Returning the merged image

if __name__ == "__main__":
    dataset_path = "dataset"
    dataset_folders = get_folder_names(dataset_path) # Getting sub-folder names in the dataset

    images = []
    for folder in dataset_folders:
        sub_images = read_images(dataset_path+"/"+folder) # Reading all images
        images.append(sub_images)

    ground_truth_homographies = read_ground_truth_homographies(dataset_path+"/"+dataset_folders[0]) # Reading ground truth homographies

    methods = ["SIFT", "SURF", "ORB"] # Setting names of the methods

    for f_id, sub_images in enumerate(images): # Looping all sub-images

        panoramas = [] # Setting an empty arrays to store computed panoramas and runtime for comparison display
        alpha_panoramas = []
        runtimes = []

        panorama, warped_images = image_merging(sub_images, ground_truth_homographies) # Getting ground truth panorama
        for i, image in enumerate(warped_images):
            save_image(image, f"{dataset_folders[f_id]}/ground_truth/warped_images/{i}")

        save_image(panorama, f"{dataset_folders[f_id]}/panorama/ground_truth") # Saving ground truth panorama

        panorama, warped_images = image_merging(sub_images, ground_truth_homographies, alpha = True)
        save_image(panorama, f"{dataset_folders[f_id]}/panorama_alpha/ground_truth")

        for method in methods: # Looping methods
            if method == "ORB": # ORB uses special matcher and less filtered parameters because it's results are already few
                homographies, runtime, keypoints, filtered_matches = iterate_method(method, sub_images, matcher = "BFHAM")
            else:
                homographies, runtime, keypoints, filtered_matches = iterate_method(method, sub_images)
            runtimes.append(runtime)

            for idx, (image, kp) in enumerate(zip(sub_images, keypoints)):
                feature_plot = cv2.drawKeypoints(image, kp, None)
                save_image(feature_plot, f"{dataset_folders[f_id]}/{method}/feature_plot/{idx}")

            for i, match in enumerate(filtered_matches):
                matching_plot = cv2.drawMatches(sub_images[0], keypoints[0], sub_images[i + 1], keypoints[i + 1], match, None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS) # Drawing lines
                save_image(matching_plot, f"{dataset_folders[f_id]}/{method}/matching_plot/{i}")

            panorama, warped_images = image_merging(sub_images, homographies)
            panoramas.append(panorama)

            for i, image in enumerate(warped_images):
                save_image(image, f"{dataset_folders[f_id]}/{method}/warped_images/{i}")

            panorama, warped_images = image_merging(sub_images, homographies, alpha = True)
            alpha_panoramas.append(panorama)
        
        for i, image in enumerate(panoramas):
            save_image(image, f"{dataset_folders[f_id]}/panorama/{methods[i]}")

        for i, image in enumerate(alpha_panoramas):
            save_image(image, f"{dataset_folders[f_id]}/panorama_alpha/{methods[i]}")
        