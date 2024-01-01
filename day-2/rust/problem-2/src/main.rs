use std::fs::File;
use std::io::prelude::*;

const INPUT_FILE: &str = "input.txt";

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
    let mut power_sum: usize = 0;

    for line in contents.split('\n') {
        if !line.is_empty() {
            let game_contents: &str;
            let mut max_red: usize = 0;
            let mut max_blue: usize = 0;
            let mut max_green: usize = 0;
            (_, game_contents) =
                line.split_at(line.find(':').expect("Coudln't split string at colon."));

            for round in game_contents.split(';') {
                for turn in round.split(',') {
                    let x = strip_and_parse(turn);
                    match get_alpha(turn).as_ref() {
                        "red" => {
                            if x > max_red {
                                max_red = x;
                            }
                        }
                        "blue" => {
                            if x > max_blue {
                                max_blue = x;
                            }
                        }
                        "green" => {
                            if x > max_green {
                                max_green = x;
                            }
                        }
                        _ => panic!("Couldn't find an identifier!"),
                    };
                }
            }
            power_sum += max_red * max_blue * max_green;
        }
    }
    power_sum
}

fn get_alpha(s: &str) -> String {
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
        assert_eq!(solution("test.txt"), 2286);
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
        assert_eq!(get_alpha("123thisisatest123",), "thisisatest");
        assert_eq!(
            get_alpha("well, certainly strange."),
            "wellcertainlystrange"
        );
    }
}
