# BBM418 Assignment 3
# Image Classification with Convolutional Neural Networks
# Kayla Akyüz
# 21726914

import os
from os import environ
import matplotlib.pyplot as plt
from PIL import Image
import random
from torch.utils.data import DataLoader, Dataset
import torch
import torch.nn as nn
import torch.optim as optim
import seaborn as sns
import numpy as np
from sklearn.metrics import confusion_matrix

environ["QT_DEVICE_PIXEL_RATIO"] = "0"
environ["QT_AUTO_SCREEN_SCALE_FACTOR"] = "1"
environ["QT_SCREEN_SCALE_FACTORS"] = "1"
environ["QT_SCALE_FACTOR"] = "1"

PRINTED_FOLDER_COUNT = 0  # This is to give output images unique name to save them all


# -------------------
# Printing Operations
# -------------------
def plot(train_losses, valid_losses, valid_accuracies, title):
    global PRINTED_FOLDER_COUNT
    # A function to plot the train and validation data according to epoch
    epochs = range(1, len(train_losses) + 1)

    plt.figure(figsize=(12, 4))
    plt.subplot(1, 2, 1)
    plt.plot(epochs, train_losses, label="Training Loss")
    plt.plot(epochs, valid_losses, label="Validation Loss")
    plt.xlabel("Epochs")
    plt.ylabel("Loss")
    plt.legend()

    plt.subplot(1, 2, 2)
    plt.plot(epochs, valid_accuracies, label="Validation Accuracy")
    plt.xlabel("Epochs")
    plt.ylabel("Accuracy")
    plt.legend()

    plt.suptitle(title)  # Adding the title

    # Saving the plot with the unique filename
    filename = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        f"image_out/plot_{PRINTED_FOLDER_COUNT}.png",
    )
    PRINTED_FOLDER_COUNT += 1

    plt.savefig(filename)
    plt.show()


def plot_text(text):
    global PRINTED_FOLDER_COUNT
    # Printing text to plots for easier viewing
    plt.figure(figsize=(6, 2))  # Setting size
    plt.axis("off")  # Turn off the axes
    plt.text(0.5, 0.5, text, ha="center", va="center", fontsize=12)

    # Saving the plot with the unique filename
    filename = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        f"image_out/plot_{PRINTED_FOLDER_COUNT}.png",
    )
    PRINTED_FOLDER_COUNT += 1
    plt.savefig(filename)
    plt.show()


def plot_confusion_matrix(y_true, y_pred, labels):
    global PRINTED_FOLDER_COUNT
    cm = confusion_matrix(y_true, y_pred, labels=labels)
    sns.heatmap(
        cm, annot=True, cmap="Blues", fmt="d", xticklabels=labels, yticklabels=labels
    )
    plt.xlabel("Predicted")
    plt.ylabel("True")

    filename = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        f"image_out/plot_{PRINTED_FOLDER_COUNT}.png",
    )
    PRINTED_FOLDER_COUNT += 1

    # Save the plot with the unique filename
    plt.savefig(filename)
    plt.show()


# -------
# Dataset
# -------
class BacteriaDataset(Dataset):
    # The dataset class to apply transform
    def __init__(self, data, transform=None):
        self.data = data
        self.transform = transform

    def __len__(self):
        return len(self.data)

    def __getitem__(self, index):
        image, label = self.data[index]
        if self.transform:
            image = self.transform(image)
        return image, label


def load_data(data_dir, transform, min_images_per_class=50, min_test_per_class=20):
    # Loading aata and returning
    categories = os.listdir(data_dir)
    data = []
    class_counts = {}  # Track the number of images per class
    for label, category in enumerate(categories):
        class_counts[label] = 0  # Initialize the count for each class
        category_dir = os.path.join(data_dir, category)
        for img_name in os.listdir(category_dir):
            img_path = os.path.join(category_dir, img_name)
            img = Image.open(img_path).convert("RGB")
            img = img.resize((128, 128))
            data.append((img, label))
            class_counts[label] += 1  # Increment the count for each image added

    # Shuffle the data once for random splitting
    random.shuffle(data)

    # Ensuring that each split has at least the required number of images per class

    train_data, valid_data, test_data = [], [], []
    class_counts_train, class_counts_valid, class_counts_test = {}, {}, {}

    for label in class_counts:
        class_counts_train[label] = 0
        class_counts_valid[label] = 0
        class_counts_test[label] = 0

    for img, label in data:
        if class_counts_train[label] < min_images_per_class:
            train_data.append((img, label))
            class_counts_train[label] += 1
        elif class_counts_valid[label] < min_test_per_class:
            valid_data.append((img, label))
            class_counts_valid[label] += 1
        elif class_counts_test[label] < min_test_per_class:
            test_data.append((img, label))
            class_counts_test[label] += 1
        else:
            test_data.append((img, label))

    return train_data, valid_data, test_data


# -----------------------------------------------------------
# Part 1: Modeling and Training a CNN classifier from Scratch
# -----------------------------------------------------------
def train(model, criterion, optimizer, train_loader, device):
    # Train the model
    model.train()
    running_loss = 0.0
    for i, (inputs, labels) in enumerate(train_loader):
        inputs, labels = inputs.to(device), labels.to(device)

        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        running_loss += loss.item()
    return running_loss / len(train_loader)


def evaluate(model, criterion, valid_loader, device):
    # Evaluate the model
    model.eval()
    running_loss = 0.0
    correct = 0
    total = 0

    with torch.no_grad():
        for inputs, labels in valid_loader:
            inputs, labels = inputs.to(device), labels.to(device)
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            running_loss += loss.item()

            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()

    accuracy = 100 * correct / total
    return running_loss / len(valid_loader), accuracy


def get_predictions(model, loader, device):
    # Getting the predictions
    model.eval()
    y_true = []
    y_pred = []

    with torch.no_grad():
        for inputs, labels in loader:
            inputs, labels = inputs.to(device), labels.to(device)
            outputs = model(inputs)
            _, predicted = torch.max(outputs.data, 1)

            y_true.extend(labels.cpu().numpy())
            y_pred.extend(predicted.cpu().numpy())

    return y_true, y_pred


def run_experiments(model, learning_rates, batch_sizes, train_set, valid_set, device):
    # Main function to run two different learning rates and two different batch sizes for the non residual model.
    # Also return best model according to losses and accuracy
    best_model_info = {"state_dict": None, "lr": None, "bs": None, "val_acc": -1}
    best_model = None  # Variable to store the best model

    model = model.to(device)  # Send model to device (GPU)
    criterion = nn.CrossEntropyLoss()  # Setting criterion to Cross Entropy Loss

    for bs in batch_sizes:
        train_loader = DataLoader(
            train_set, batch_size=bs, shuffle=True
        )  # Loading data
        valid_loader = DataLoader(valid_set, batch_size=bs, shuffle=False)

        for lr in learning_rates:
            optimizer = optim.Adam(
                model.parameters(), lr=lr
            )  # Setting optimizer to Adam

            train_losses = []
            valid_losses = []
            valid_accuracies = []
            for epoch in range(1, 101):  # For epoch in length 100
                train_loss = train(
                    model, criterion, optimizer, train_loader, device
                )  # Training the model
                valid_loss, valid_accuracy = evaluate(
                    model, criterion, valid_loader, device
                )  # Evaluating the model

                train_losses.append(train_loss)  # Registering loss and accuracy
                valid_losses.append(valid_loss)
                valid_accuracies.append(valid_accuracy)

                if (
                    valid_accuracy > best_model_info["val_acc"]
                ):  # Checking for best model and recording the data of best model
                    best_model_info["state_dict"] = model.state_dict()
                    best_model_info["lr"] = lr
                    best_model_info["bs"] = bs
                    best_model_info["val_acc"] = valid_accuracy
                    best_model = model

            plot(
                train_losses,
                valid_losses,
                valid_accuracies,
                "Learning Rate: {} - Batch Size: {}".format(lr, bs),
            )  # Plotting the result for the current learning rate and batch

    return best_model, best_model_info


class ResidualBlock(nn.Module):
    def __init__(self, in_channels, out_channels, stride=1):
        super().__init__()
        self.conv1 = nn.Conv2d(
            in_channels, out_channels, kernel_size=3, stride=stride, padding=1
        )
        self.bn1 = nn.BatchNorm2d(out_channels)
        self.relu = nn.ReLU()
        self.conv2 = nn.Conv2d(
            out_channels, out_channels, kernel_size=3, stride=1, padding=1
        )
        self.bn2 = nn.BatchNorm2d(out_channels)

        self.shortcut = nn.Sequential()
        if stride != 1 or in_channels != out_channels:
            self.shortcut = nn.Sequential(
                nn.Conv2d(
                    in_channels, out_channels, kernel_size=1, stride=stride, padding=0
                ),
                nn.BatchNorm2d(out_channels),
            )

    def forward(self, x):
        out = self.relu(self.bn1(self.conv1(x)))
        out = self.bn2(self.conv2(out))
        out += self.shortcut(x)
        out = self.relu(out)
        return out


class ResNet(nn.Module):
    def __init__(self, num_classes=8):
        super().__init__()
        self.conv1 = nn.Conv2d(3, 32, kernel_size=3, stride=1, padding=1)
        self.bn1 = nn.BatchNorm2d(32)
        self.relu = nn.ReLU()
        self.maxpool = nn.MaxPool2d(kernel_size=2, stride=2)
        self.layer1 = self._make_layer(32, 64, 1, stride=1)
        self.layer2 = self._make_layer(64, 128, 1, stride=1)
        self.layer3 = self._make_layer(128, 256, 1, stride=1)
        self.layer4 = self._make_layer(256, 512, 1, stride=1)
        self.layer5 = self._make_layer(512, 1024, 1, stride=1)
        self.fc = nn.Linear(1024, num_classes)

    def _make_layer(self, in_channels, out_channels, num_blocks, stride):
        strides = [stride] + [1] * (num_blocks - 1)
        layers = []
        for stride in strides:
            layers.append(ResidualBlock(in_channels, out_channels, stride))
            in_channels = out_channels
        return nn.Sequential(*layers)

    def forward(self, x):
        out = self.relu(self.bn1(self.conv1(x)))
        out = self.maxpool(out)
        out = self.layer1(out)
        out = self.layer2(out)
        out = self.layer3(out)
        out = self.layer4(out)
        out = self.layer5(out)
        out = nn.AdaptiveAvgPool2d((1, 1))(out)
        out = out.view(out.size(0), -1)
        out = self.fc(out)
        return out


class ResidualBlockDropout(nn.Module):
    # Modify the ResidualBlock and ResNet classes to include dropout
    def __init__(self, in_channels, out_channels, stride=1, dropout_value=0.5):
        super().__init__()
        self.dropout_value = dropout_value
        self.conv1 = nn.Conv2d(
            in_channels, out_channels, kernel_size=3, stride=stride, padding=1
        )
        self.bn1 = nn.BatchNorm2d(out_channels)
        self.relu = nn.ReLU()
        self.dropout = nn.Dropout(dropout_value)
        self.conv2 = nn.Conv2d(
            out_channels, out_channels, kernel_size=3, stride=1, padding=1
        )
        self.bn2 = nn.BatchNorm2d(out_channels)

        self.shortcut = nn.Sequential()
        if stride != 1 or in_channels != out_channels:
            self.shortcut = nn.Sequential(
                nn.Conv2d(
                    in_channels, out_channels, kernel_size=1, stride=stride, padding=0
                ),
                nn.BatchNorm2d(out_channels),
            )

    def forward(self, x):
        out = self.relu(self.bn1(self.conv1(x)))
        out = self.dropout(out)
        out = self.bn2(self.conv2(out))
        out += self.shortcut(x)
        out = self.relu(out)
        return out


class ResNetDropout(nn.Module):
    def __init__(self, num_classes=8, dropout_value=0.5):
        super().__init__()
        self.conv1 = nn.Conv2d(3, 32, kernel_size=3, stride=1, padding=1)
        self.bn1 = nn.BatchNorm2d(32)
        self.relu = nn.ReLU()
        self.maxpool = nn.MaxPool2d(kernel_size=2, stride=2)
        self.layer1 = self._make_layer(32, 64, 1, stride=1, dropout_value=dropout_value)
        self.layer2 = self._make_layer(
            64, 128, 1, stride=1, dropout_value=dropout_value
        )
        self.layer3 = self._make_layer(
            128, 256, 1, stride=1, dropout_value=dropout_value
        )
        self.layer4 = self._make_layer(
            256, 512, 1, stride=1, dropout_value=dropout_value
        )
        self.layer5 = self._make_layer(
            512, 1024, 1, stride=1, dropout_value=dropout_value
        )
        self.fc = nn.Linear(1024, num_classes)

    def _make_layer(self, in_channels, out_channels, num_blocks, stride, dropout_value):
        strides = [stride] + [1] * (num_blocks - 1)
        layers = []
        for stride in strides:
            layers.append(
                ResidualBlockDropout(in_channels, out_channels, stride, dropout_value)
            )
            in_channels = out_channels
        return nn.Sequential(*layers)

    def forward(self, x):
        out = self.relu(self.bn1(self.conv1(x)))
        out = self.maxpool(out)
        out = self.layer1(out)
        out = self.layer2(out)
        out = self.layer3(out)
        out = self.layer4(out)
        out = self.layer5(out)
        out = nn.AdaptiveAvgPool2d((1, 1))(out)
        out = out.view(out.size(0), -1)
        out = self.fc(out)
        return out


# -----------------------------------
# Part 2: Transfer Learning with CNNs
# -----------------------------------
def train_and_evaluate(
    model, optimizer, criterion, train_loader, valid_loader, device, num_epochs=100
):
    train_losses, valid_losses, valid_accuracies = [], [], []

    for epoch in range(num_epochs):
        # Training
        model.train()
        train_loss = 0
        for images, labels in train_loader:
            images, labels = images.to(device), labels.to(device)

            optimizer.zero_grad()
            outputs = model(images)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            train_loss += loss.item()

        train_losses.append(train_loss / len(train_loader))

        # Validation
        model.eval()
        valid_loss, correct, total = 0, 0, 0
        with torch.no_grad():
            for images, labels in valid_loader:
                images, labels = images.to(device), labels.to(device)

                outputs = model(images)
                loss = criterion(outputs, labels)

                valid_loss += loss.item()
                _, predicted = torch.max(outputs.data, 1)
                total += labels.size(0)
                correct += (predicted == labels).sum().item()

        valid_losses.append(valid_loss / len(valid_loader))
        valid_accuracies.append(correct / total)

    return train_losses, valid_losses, valid_accuracies


def get_pred_part2(model, test_loader, device):
    y_true = []
    y_pred = []

    with torch.no_grad():
        for images, labels in test_loader:
            images = images.to(device)
            labels = labels.to(device)

            outputs = model(images)
            _, predicted = torch.max(outputs, 1)

            y_true.extend(labels.cpu().numpy())
            y_pred.extend(predicted.cpu().numpy())

    # Convert the lists to numpy arrays
    y_true = np.array(y_true)
    y_pred = np.array(y_pred)

    return y_true, y_pred
