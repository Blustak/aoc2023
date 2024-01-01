use std::fs::File;
use std::io::prelude::*;

const INPUT_PATH: &str = "input.txt";

fn main() {
    println!("I got {} as the final sum", solution(INPUT_PATH));
}

fn solution(file_path: &str) -> u32 {
    //File handling
    let number_s = [
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
    ];
    let mut file = File::open(file_path).expect("Couldn't open file.");
    let mut contents: String = String::new();
    let mut sum: u32 = 0;
    file.read_to_string(&mut contents)
        .expect("Error reading to string");

    //Solution
    for line in contents.split('\n').map(|line| line.trim()) {
        if !(line.is_empty()) {
            let mut first: u32 = 0;
            let mut first_pos = usize::MAX;
            let mut last: u32 = 0;
            let mut last_pos = usize::MIN;
            for number in 0..number_s.len() {
                let no = number_s[number];
                if let Some(x) = line.find(no) {
                    if x < first_pos {
                        first = number as u32 + 1;
                        first_pos = x;
                    }
                }
                if let Some(x) = line.rfind(no) {
                    if x > last_pos || last == 0 {
                        //last == 0 needed in case last digit is in 0th
                        //position
                        last = number as u32 + 1;
                        last_pos = x;
                    }
                }
                for c in line.char_indices() {
                    match c.1 {
                        '0'..='9' => {
                            if c.0 < first_pos {
                                first = c.1.to_digit(10).unwrap();
                                break;
                            }
                        }
                        _ => continue,
                    }
                }
                for c in line.char_indices().rev() {
                    match c.1 {
                        '0'..='9' => {
                            if c.0 > last_pos || last == 0 {
                                last = c.1.to_digit(10).unwrap();
                                break;
                            }
                        }
                        _ => continue,
                    }
                }
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
        assert_eq!(solution("test.txt"), 281);
    }
}
