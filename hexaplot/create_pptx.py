import os

from pptx import Presentation
from pptx.util import Inches


# Function to add a slide with 6 images
def add_slide_with_images(prs, image_paths, title_text):
    slide = prs.slides.add_slide(prs.slide_layouts[5])  # Blank layout
    title = slide.shapes.title
    title.text = title_text

    # Define positions for the 6 images (3x2 grid)
    positions = [
        (Inches(0.5), Inches(1.5), Inches(4), Inches(3)),  # Top-left
        (Inches(5), Inches(1.5), Inches(4), Inches(3)),  # Top-center
        (Inches(9.5), Inches(1.5), Inches(4), Inches(3)),  # Top-right
        (Inches(0.5), Inches(4.75), Inches(4), Inches(3)),  # Bottom-left
        (Inches(5), Inches(4.75), Inches(4), Inches(3)),  # Bottom-center
        (Inches(9.5), Inches(4.75), Inches(4), Inches(3)),  # Bottom-right
    ]

    for pos, image_path in zip(positions, image_paths):
        left, top, width, height = pos
        slide.shapes.add_picture(image_path, left, top, width, height)


# Create PowerPoint presentation
prs = Presentation()
prs.slide_width = Inches(13.33)  # Set width for 16:9
prs.slide_height = Inches(7.5)  # Set height for 16:9
title_slide = prs.slides.add_slide(prs.slide_layouts[0])
title_slide.shapes.title.text = "Plot Overview"

os.chdir("./Pedestals/")
# Example image paths
image_sets = [
    [
        "Pedestals_BV_-130_Module_0.png",
        "Pedestals_BV_-130_Module_1.png",
        "Pedestals_BV_-130_Module_2.png",
        "Pedestals_BV_-130_Module_3.png",
        "Pedestals_BV_-130_Module_4.png",
        "Pedestals_BV_-130_Module_5.png",
    ],
    [
        "Pedestals_BV_-300_Module_0.png",
        "Pedestals_BV_-300_Module_1.png",
        "Pedestals_BV_-300_Module_2.png",
        "Pedestals_BV_-300_Module_3.png",
        "Pedestals_BV_-300_Module_4.png",
        "Pedestals_BV_-300_Module_5.png",
    ],
    [
        "Pedestals_BV_-400_Module_0.png",
        "Pedestals_BV_-400_Module_1.png",
        "Pedestals_BV_-400_Module_2.png",
        "Pedestals_BV_-400_Module_3.png",
        "Pedestals_BV_-400_Module_4.png",
        "Pedestals_BV_-400_Module_5.png",
    ],
    [
        "Pedestals_BV_-500_Module_0.png",
        "Pedestals_BV_-500_Module_1.png",
        "Pedestals_BV_-500_Module_2.png",
        "Pedestals_BV_-500_Module_3.png",
        "Pedestals_BV_-500_Module_4.png",
        "Pedestals_BV_-500_Module_5.png",
    ],
    [
        "Pedestals_BV_-600_Module_0.png",
        "Pedestals_BV_-600_Module_1.png",
        "Pedestals_BV_-600_Module_2.png",
        "Pedestals_BV_-600_Module_3.png",
        "Pedestals_BV_-600_Module_4.png",
        "Pedestals_BV_-600_Module_5.png",
    ],
    [
        "Pedestals_BV_-700_Module_0.png",
        "Pedestals_BV_-700_Module_1.png",
        "Pedestals_BV_-700_Module_2.png",
        "Pedestals_BV_-700_Module_3.png",
        "Pedestals_BV_-700_Module_4.png",
        "Pedestals_BV_-700_Module_5.png",
    ],
]

# Add slides for each set
for idx, image_paths in enumerate(image_sets):
    add_slide_with_images(prs, image_paths, f"Set {idx + 1}")

# Save the presentation
prs.save("plots_presentation.pptx")
print("Presentation saved as plots_presentation.pptx")
