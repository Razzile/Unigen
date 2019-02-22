pub mod metadata;

use metadata::metadata::*;

pub fn load_metadata(path: &str) {
    match MetadataObject::from_file(path) {
        Err(error) => {
            println!("{}", error.to_string());
        },
        Ok(object) => {
            println!("metadata version {}", object.version);
        }
    }
}