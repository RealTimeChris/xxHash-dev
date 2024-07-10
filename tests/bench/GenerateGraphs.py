import argparse
import json
import os
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import re

# Set up command-line argument parsing
def parse_args():
    parser = argparse.ArgumentParser(description='Process benchmark results from a JSON file.')
    parser.add_argument('input_file', type=str, help='Path to the input JSON file')
    parser.add_argument('output_folder', type=str, help='Path to the output directory')
    return parser.parse_args()

def main():
    args = parse_args()
    input_file = args.input_file
    output_folder = args.output_folder

    # Load data from the specified JSON file
    with open(input_file, 'r') as file:
        data = json.load(file)

    os.makedirs(output_folder, exist_ok=True)

    df = pd.json_normalize(data, 'results')

    def sanitize_filename(filename):
        return re.sub(r'[<>:"/\\|?*]', ' ', filename)

    def plot_and_save_benchmarks(df):
        tests = df["benchmarkName"].unique()

        for test in tests:
            sns.set_style("dark")
            sns.set_style(rc={'axes.facecolor': '#0d1117'})
            plt.figure(figsize=(10, 6))
            ax = plt.gca()

            test_data = df[df["benchmarkName"] == test]

            for i, library in enumerate(test_data["libraryName"].unique()):
                library_data = test_data[test_data["libraryName"] == library]
                if not library_data.empty:
                    color = library_data["benchmarkColor"].iloc[0]
                    time = library_data["resultTime"].iloc[0]
                    ax.bar(i, time, color=color, label=library)

            # Collect the units from the first entry for setting the y-axis label
            units = test_data["units"].iloc[0] if "units" in test_data.columns else "ms"

            ax.set_xticks(range(len(test_data["libraryName"].unique())))
            ax.set_xticklabels(test_data["libraryName"].unique(), color='lightgray')
            ax.set_title(f'{test} Results', color='white')
            ax.set_xlabel('Library Name', color='white')
            ax.set_ylabel(f'Result Time ({units})', color='white')

            sns.set_theme(style="whitegrid", rc={"axes.edgecolor": "#0d1117", "xtick.color": "#0d1117", "ytick.color": "#0d1117"})
            plt.gcf().set_facecolor("#0d1117")

            for text in ax.get_xticklabels() + ax.get_yticklabels():
                text.set_color('lightgray')

            ax.legend(title='Library', loc='best')
            sanitized_test_name = sanitize_filename(test)
            filename = f'{sanitized_test_name}_results.jpg'

            # Save the plot as a .jpg file
            plt.savefig(os.path.join(output_folder, filename), format='jpg')
            plt.close()

    # Call the function with the DataFrame
    plot_and_save_benchmarks(df)

if __name__ == "__main__":
    main()
