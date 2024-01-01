use std::fs::File;
use std::io::prelude::*;

const INPUT_FILE: &str = "input.txt";
const RED_CUBES: usize = 12;
const BLUE_CUBES: usize = 14;
const GREEN_CUBES: usize = 13;

fn main() {
    println!("I got {} as my solution.", solution(INPUT_FILE));
}

fn solution(file_path: &str) -> usize {
    //File handling
    let mut file = File::open(file_path).expect("Sorry, couldn't read the input file.");
    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .expect("Couldn't read file contents.");

    //Solution
    let mut id_sum: usize = 0;

    for line in contents.split('\n') {
        if !line.is_empty() {
            let game_id: &str;
            let game_contents: &str;
            (game_id, game_contents) =
                line.split_at(line.find(':').expect("Coudln't split string at colon."));
            let id: usize = strip_and_parse(game_id);
            if game_contents.split(';').all(|round| {
                round
                    .split(',')
                    .all(|turn| match get_non_alpha(turn).as_ref() {
                        "red" => strip_and_parse(turn) <= RED_CUBES,
                        "blue" => strip_and_parse(turn) <= BLUE_CUBES,
                        "green" => strip_and_parse(turn) <= GREEN_CUBES,
                        _ => panic!("Somehow given non rgb input."),
                    })
            }) {
                println!("Game {} was acceptable!", id);
                id_sum += id;
            }
        }
    }
    id_sum
}

fn get_non_alpha(s: &str) -> String {
    s.chars().filter(|c| c.is_alphabetic()).collect()
}

fn strip_and_parse(s: &str) -> usize {
    s.chars()
        .filter(|c| c.is_digit(10))
        .collect::<String>()
        .parse()
        .unwrap()
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn test_input() {
        assert_eq!(solution("test.txt"), 8);
    }
    #[test]
    fn test_strip_and_parse() {
        assert_eq!(strip_and_parse("12ab3"), 123);
        assert_eq!(
            strip_and_parse("00001there once was a man from kent2ky, and he loved to eat worm3."),
            123
        )
    }
    #[test]
    fn test_strip() {
        assert_eq!(get_non_alpha("123thisisatest123",), "thisisatest");
        assert_eq!(
            get_non_alpha("well, certainly strange."),
            "wellcertainlystrange"
        );
    }
}
