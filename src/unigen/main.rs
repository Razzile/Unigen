extern crate libunigen;

use clap::{load_yaml, App};

fn main() {
    let config = load_yaml!("../../config.yml");
    let matches = App::from_yaml(config).get_matches();

    let binary = matches.value_of("binary").unwrap();
    let metadata = matches.value_of("metadata").unwrap();

    println!("binary: {} metadata: {}", binary, metadata);

    libunigen::load_metadata(metadata);
}