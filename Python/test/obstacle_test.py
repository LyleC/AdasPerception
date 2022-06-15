import sys
from pathlib import Path

sys.path.append(str(Path(__file__).resolve().parents[1]))

from src.targets.obstacles import Obstacle


def test_obstacle():
    obst = Obstacle()
    assert True
    return
