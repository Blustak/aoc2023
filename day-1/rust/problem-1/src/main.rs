use std::fs::File;
use std::io::prelude::*;

const INPUT_PATH: &str = "input.txt";

fn main() {
    println!("I got {} as the final sum", solution(INPUT_PATH));
}

fn solution(file_path: &str) -> u32 {
    //File handling
    let mut file = File::open(file_path).expect("Couldn't open file.");
    let mut contents: String = String::new();
    let mut sum: u32 = 0;
    file.read_to_string(&mut contents)
        .expect("Error reading to string");

    //Solution
    for line in contents.split('\n').map(|line| line.trim()) {
        if !(line.is_empty()) {
            let mut first: u32 = 0;
            let mut last: u32 = 0;
            for c in line.chars() {
                match c {
                    '0'..='9' => {
                        first = c.to_digit(10).unwrap();
                        break;
                    }
                    _ => {
                        continue;
                    }
                }
            }
            for c in line.chars().rev() {
                match c {
                    '0'..='9' => {
                        last = c.to_digit(10).unwrap();
                        break;
                    }
                    _ => continue,
                }
            }
            if first == 0 || last == 0 {
                panic!("couldn't find a matching digit!");
            }
            sum += first * 10 + last;
        }
    }
    sum
}

#[cfg(test)]
mod tests {
    #[test]
    fn sample_input() {
        use super::solution;
        assert_eq!(solution("test.txt"), 142);
    }
}
