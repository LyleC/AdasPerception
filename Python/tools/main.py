import sys
from pathlib import Path

sys.path.append(str(Path(__file__).resolve().parents[1]))

import fire

from src.utils.io_funcs import *
from src.process import run_perception


def main(cfg_file):
    cfg = decode_yaml_config(cfg_file)
    vcap = decode_video(cfg.inputs.video)
    results = decode_predict_results(cfg.inputs.predict)
    data_check(vcap, results)

    run_perception(vcap, results, cfg)
    return


if __name__ == "__main__":
    fire.Fire(main)
