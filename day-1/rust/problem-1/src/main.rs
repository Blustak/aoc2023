use std::env;
use std::fs;

fn main() {
    let mut sum: u32 = 0;
    let args: Vec<String> = env::args().collect();
    for arg in args.iter() {
        println!("{}", arg);
    }
    let lines: Vec<&str>;
    if let Ok(contents) = read_file(&args[1]) {
        lines = contents.split('\n').collect();
        for line in lines.iter() {
            if !(line.is_empty()) {
                let digits: Vec<char> = line.chars().filter(|c| c.is_digit(10)).collect();
                sum += digits
                    .first()
                    .unwrap()
                    .to_digit(10)
                    .expect("Couldn't convert to digit")
                    * 10
                    + digits
                        .last()
                        .unwrap()
                        .to_digit(10)
                        .expect("Couldn't convert to digit");
            }
        }
    } else {
        panic!("Sorry, couldn't read file");
    }
    println!("The final sum i got was {}", sum);
}

fn read_file(file_path: &str) -> Result<String, std::io::Error> {
    fs::read_to_string(file_path)
}
