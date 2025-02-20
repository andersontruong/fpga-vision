#!/usr/bin/env python3
from ultralytics import YOLO
import matplotlib.pyplot as plt
import cv2
import torch

def main():
    # Check GPU availability
    if torch.cuda.is_available():
        print("GPU available: Using GPU for training.")
    else:
        print("GPU not available: Training will run on CPU.")

    # ------------------------------------------------------------------------------
    # Step 1: Load a COCO-pretrained YOLOv8 model (e.g., YOLOv8n for a nano model)
    # ------------------------------------------------------------------------------
    # YOLO will automatically use the GPU if available.
    model = YOLO("yolo11n.pt")
    model.info()

    # ------------------------------------------------------------------------------
    # Step 2: Fine-tune (Transfer Learning) on your custom dataset
    # ------------------------------------------------------------------------------
    results = model.train(
        data="10k.yaml",        # YAML file defining train/val/test directories and classes
        epochs=10,              # Number of training epochs
        imgsz=640,              # Input image size
        lr0=0.01,               # Initial learning rate
        project="runs/exp_10k", # Directory to save training results
        name="yolo11n_10k"      # Name of this training run
    )

    # Save the fine-tuned model weights for later use
    model.save("yolo11n_10k_finetuned.pt")

    # ------------------------------------------------------------------------------
    # Step 3: Run Inference on a Sample Image (e.g., a test image from your dataset)
    # ------------------------------------------------------------------------------
    inference_results = model("sim_ss.png", conf=0.4, iou=0.4)

    # ------------------------------------------------------------------------------
    # Step 4: Visualize (Save) the Inference Results
    # ------------------------------------------------------------------------------
    # Since this script runs in the background, we save figures instead of showing them.
    for idx, result in enumerate(inference_results):
        # Get the annotated image with bounding boxes
        annotated_img = result.plot()
        # Convert from BGR (OpenCV default) to RGB for correct color display
        annotated_img_rgb = cv2.cvtColor(annotated_img, cv2.COLOR_BGR2RGB)
        plt.figure(figsize=(10, 8))
        plt.imshow(annotated_img_rgb)
        plt.title("Inference Result with Bounding Boxes")
        plt.axis('off')
        # Save the figure to a file
        plt.savefig(f"inference_result_{idx}.png")
        plt.close()

if __name__ == "__main__":
    main()