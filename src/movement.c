#include "game_params.h"
#include "placement.h"
#include "printer.h"
#include <stdio.h>
#include <stdlib.h>
#include "read_values.h"
#include "movement.h"
#include "validations.h"

void movement(GameParams *game_params) {

	int flag = 0;
	int* source = (int*)malloc(3 * sizeof(int));
	int* destination = (int*)malloc(3 * sizeof(int));
	const char* clear = "clear";

	while (flag < game_params->player_count) {
		for (int j = 0; j < game_params->player_count; ++j) {
			if (player_can_move(j, game_params)) {

				print_stage(game_params);
				print_scoreboard(game_params);

				printf("Player %d: \n ", (j + 1));
				printf("\t please select a penguin to move: \n\t ");

				read_source(game_params, source, j);
				read_destination(game_params, source, destination);

				system(clear);

				perform_move(source, destination, game_params, j);

			} else {
				flag++;
			}
		}
	}

	printf("end of game");

	free(source);
	free(destination);
}

void read_source(GameParams *game_params, int* source, int current_player) {
	int error_count;
	do {
		error_count = 0;
		error_count += read_values(source);
		error_count += validate_x(game_params, source);
		error_count += validate_y(game_params, source);
		error_count += validate_penguin(game_params, current_player, source);
		if (validate_penguin_can_move(game_params, source) == 0) {
			error_count++;
		}
	} while (error_count > 0);
}

void read_destination(GameParams *game_params, int* source, int* destination) {
	int error_count;
	do {
		error_count = 0;
		error_count += read_values(destination);
		error_count += validate_x(game_params, destination);
		error_count += validate_y(game_params, destination);
		error_count += validate_movement(game_params, source, destination, 1);
	} while (error_count > 0);
}

void perform_move(int* source, int* destination, GameParams *game_params, int current_player) {
	game_params->scoreboard_array[current_player] += game_params->board[destination[0]][destination[1]];
	game_params->board[source[0]][source[1]] = 0;
	game_params->board[destination[0]][destination[1]] = game_params->player_array[current_player];
}