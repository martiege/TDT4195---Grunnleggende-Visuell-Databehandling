# It should not be required to change this code
import torch
import matplotlib.pyplot as plt


def compute_loss_and_accuracy(dataloader, model, loss_function):
    """
    Computes the total loss and accuracy over the whole dataloader
    Args:
        dataloder: Test dataloader
        model: torch.nn.Module
        loss_function: The loss criterion, e.g: nn.CrossEntropyLoss()
    Returns:
        [loss_avg, accuracy]: both scalar.
    """
    model.eval()
    # Tracking variables
    loss_avg = 0
    total_correct = 0
    total_images = 0
    total_steps = 0
    with torch.no_grad():  # No need to compute gradient when testing
        for (X_batch, Y_batch) in dataloader:
            # Forward pass the images through our model
            output_probs = model(X_batch)
            # Compute loss
            loss = loss_function(output_probs, Y_batch)

            # Predicted class is the max index over the column dimension
            predictions = output_probs.argmax(dim=1).squeeze()
            Y_batch = Y_batch.squeeze()

            # Update tracking variables
            loss_avg += loss.item()
            total_steps += 1
            total_correct += (predictions == Y_batch).sum().item()
            total_images += predictions.shape[0]
    model.train()
    loss_avg = loss_avg / total_steps
    accuracy = total_correct / total_images
    return loss_avg, accuracy


def plot_loss(loss_dict, label):
    global_steps = list(loss_dict.keys())
    loss = list(loss_dict.values())
    plt.plot(global_steps, loss, label=label)
