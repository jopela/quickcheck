Gem::Specification.new do |s|
  s.name = "quickcheck"
  s.version = "0.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Philip Tolton"]
  s.date = "2012-08-09"
  s.description = "Quickly check if two places share a similar word."
  s.email = "mrtolton@gmail.com"
  s.extensions = ["ext/extconf.rb"]
  s.files = [
    "ext/extconf.rb",
    "ext/quickcheck.c",
  ]
  s.summary = "C version of most frequently called function in the linking process."
end