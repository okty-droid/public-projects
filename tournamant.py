# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    counts = {}

    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for team in reader:
            team["rating"] = int(team["rating"])
            teams.append(team)
    # print(teams)

    # print(teams[1]['rating']) # works

    for t in teams:
        s = t["team"]
        counts[s] = 0

    # TODO: Simulate N tournaments and keep track of win counts
    for j in range(N):
        winner = simulate_tournament(teams)
        if winner in counts:
            counts[winner] += 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""

    left = teams
    while (len(left) > 1):
        left = simulate_round(left)
    # left is returned as a dictionary entry with team and rating
    return left[0]["team"]

    # TODO


if __name__ == "__main__":
    main()
